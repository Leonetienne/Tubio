#include "DownloadManager.h"

using namespace Downloader;
using namespace JasonPP;

void DownloadManager::PreInit()
{
	log = new Logging::Logger("DownloadManager");

	FileSystem::CreateDirectoryIfNotExists(XGConfig::downloader.cachedir);
	FileSystem::CreateDirectoryIfNotExists(XGConfig::downloader.cachedir + "/metadata");
	FileSystem::CreateDirectoryIfNotExists(XGConfig::downloader.cachedir + "/download");
	FileSystem::CreateDirectoryIfNotExists(XGConfig::downloader.cachedir + "/dlprogbuf");

	Load();

	return;
}

std::string DownloadManager::QueueDownload(std::string url, DOWNLOAD_MODE mode)
{	
	std::string tubioId = CreateNewTubioID();
	FetchInformation(url, tubioId);

	std::string jsString = FileSystem::ReadFile(XGConfig::downloader.cachedir + "/metadata/" + tubioId + ".json");

	DownloadEntry newDownload;
	newDownload.tubio_id = tubioId;
	newDownload.mode = mode;
	newDownload.download_progress = 0;
	newDownload.queued_timestamp = time(0);
	newDownload.download_url = "/download/" + newDownload.tubio_id;

	WarnIfMissingDependenciesWIN();

	if (!IsJsonValid(jsString))
	{
		newDownload.status = DOWNLOAD_STATUS::FAILED;
	}
	else
	{
		newDownload.status = DOWNLOAD_STATUS::QUEUED;
		Json j;
		j.Parse(jsString);
		if (j.GetDataType() != JDType::JSON)
		{
			newDownload.status = DOWNLOAD_STATUS::FAILED;
		}
		else
		{
			if ((j.AsJson.DoesExist("title")) && (j.AsJson["title"].GetDataType() == JDType::STRING))
			{
				newDownload.title = j["title"].AsString;
			}

			if ((j.AsJson.DoesExist("description")) && (j.AsJson["description"].GetDataType() == JDType::STRING))
			{
				newDownload.description = j["description"].AsString;
			}

			if ((j.AsJson.DoesExist("uploader")) && (j.AsJson["uploader"].GetDataType() == JDType::STRING))
			{
				newDownload.uploader = j["uploader"].AsString;
			}

			if ((j.AsJson.DoesExist("duration")) && (j.AsJson["duration"].GetDataType() == JDType::INT))
			{
				newDownload.duration = (uint32_t)j["duration"].AsInt;
			}

			if ((j.AsJson.DoesExist("webpage_url")) && (j.AsJson["webpage_url"].GetDataType() == JDType::STRING))
			{
				newDownload.webpage_url = j["webpage_url"].AsString;
			}

			if ((j.AsJson.DoesExist("thumbnails")) && (j.AsJson["thumbnails"].GetDataType() == JDType::ARRAY))
			{
				JsonArray& thumbnails = j.AsJson["thumbnails"].AsArray;
				if (thumbnails.Size() > 0)
				{
					if (thumbnails.Size() > 1)
					{
						// If we have more than one thumbnail to choose from, choose the second-highes quality.
						newDownload.thumbnail_url = thumbnails[thumbnails.Size() - 2]["url"].AsString;
					}
					else
					{
						newDownload.thumbnail_url = thumbnails[thumbnails.Size() - 1]["url"].AsString;
					}
				}
			}
		}
	}

	unfinishedCache.push_back(newDownload);

	return tubioId;
}

void DownloadManager::Update()
{
	if (shouldSave) Save();

	std::size_t cachedNumActiveDownloads = GetNumActiveDownloads();

	// Queue next download, if available
	if ((long long)cachedNumActiveDownloads < (long long)XGConfig::downloader.num_threads)
	{
		DownloadNext();
	}

	// Update every 1-2 seconds, non-blocking, the download progresses 
	if ((time(0) - lastProgressCheck > 2) && (cachedNumActiveDownloads > 0))
	{
		// Mutex gets reset in Save();
		UpdateDownloadProgressPercentages();
	}

	// Clear cache, if requested
	if ((shouldClearCacheASAP) && (cachedNumActiveDownloads == 0))
	{
		shouldClearCacheASAP = false;
		ClearDownloadCache();
	}

	return;
}

void DownloadManager::DownloadNext()
{
	// Abort, if queue is empty
	if (GetQueueLength() == 0) return;

	DownloadEntry* next = nullptr;

	for (std::size_t i = 0; i < unfinishedCache.size(); i++)
	{
		if (unfinishedCache[i].status == DOWNLOAD_STATUS::QUEUED)
		{
			next = &unfinishedCache[i];
			break;
		}
	}
	if (next)
		next->status = DOWNLOAD_STATUS::DOWNLOADING;

	std::thread* downloadThread = new std::thread([=]() {
		DownloadEntry* entry = next;

		std::stringstream ss;
		if (entry->mode == DOWNLOAD_MODE::VIDEO)
		{
			std::string ytdl_call_video_base = 
				"youtube-dl --newline --no-call-home --no-playlist --no-part --no-warnings --socket-timeout 5 --limit-rate $$DL_RATE"
				" --no-mtime --no-cache-dir --recode-video mp4 --prefer-ffmpeg"
				" -o \"$$DL_FILE\" \"$$DL_URL\" > \"$$DL_PROG_BUF_FILE\"";


			ytdl_call_video_base = Internal::StringHelpers::Replace(ytdl_call_video_base, "$$DL_RATE", XGConfig::downloader.max_dlrate_per_thread);
			ytdl_call_video_base = Internal::StringHelpers::Replace(ytdl_call_video_base, "$$DL_FILE", XGConfig::downloader.cachedir + "/download/" + entry->tubio_id + ".%(ext)s");
			ytdl_call_video_base = Internal::StringHelpers::Replace(ytdl_call_video_base, "$$DL_URL", entry->webpage_url);
			ytdl_call_video_base = Internal::StringHelpers::Replace(ytdl_call_video_base, "$$DL_PROG_BUF_FILE", XGConfig::downloader.cachedir + "/dlprogbuf/" + entry->tubio_id + ".buf");


			entry->downloaded_filename = XGConfig::downloader.cachedir + "/download/" + entry->tubio_id + ".mp4";
			ss << ytdl_call_video_base;
		}
		else // DOWNLOAD_MODE::AUDIO
		{
			std::string ytdl_call_audio_base =
				"youtube-dl --newline --no-call-home --no-playlist --no-part --no-warnings --socket-timeout 5 --limit-rate $$DL_RATE"
				" --no-mtime --no-cache-dir --audio-format mp3 --audio-quality 0 --extract-audio -o \"$$DL_FILE\""
				" \"$$DL_URL\" > \"$$DL_PROG_BUF_FILE\"";

			ytdl_call_audio_base = Internal::StringHelpers::Replace(ytdl_call_audio_base, "$$DL_RATE", XGConfig::downloader.max_dlrate_per_thread);
			ytdl_call_audio_base = Internal::StringHelpers::Replace(ytdl_call_audio_base, "$$DL_FILE", XGConfig::downloader.cachedir + "/download/" + entry->tubio_id + ".%(ext)s");
			ytdl_call_audio_base = Internal::StringHelpers::Replace(ytdl_call_audio_base, "$$DL_URL", entry->webpage_url);
			ytdl_call_audio_base = Internal::StringHelpers::Replace(ytdl_call_audio_base, "$$DL_PROG_BUF_FILE", XGConfig::downloader.cachedir + "/dlprogbuf/" + entry->tubio_id + ".buf");
		

			entry->downloaded_filename = XGConfig::downloader.cachedir + "/download/" + entry->tubio_id + ".mp3";
			ss << ytdl_call_audio_base;
		}

		int returnCode = system(ss.str().c_str());

		if (returnCode == 0)
		{
			// Download succeeded
			entry->status = DOWNLOAD_STATUS::FINISHED;
			entry->download_progress = 100;
			shouldSave = true;
		}
		else
		{
			// Download failed
			entry->status = DOWNLOAD_STATUS::FAILED;
			entry->download_progress = -1;
		}
		return;
	});

	downloadThreads.push_back(downloadThread);

	return;
}

void DownloadManager::UpdateDownloadProgressPercentages()
{
	for (std::size_t i = 0; i < unfinishedCache.size(); i++)
	{
		if (unfinishedCache[i].status == DOWNLOAD_STATUS::DOWNLOADING)
		{
			std::string filePath = XGConfig::downloader.cachedir + "/dlprogbuf/" + unfinishedCache[i].tubio_id + ".buf";
			if (FileSystem::Exists(filePath))
			{
				std::ifstream ifs;
				ifs.open(filePath, std::ios::in);
				if (ifs.good())
				{
					std::string lbuf;
					while (std::getline(ifs, lbuf))
					{
						if (lbuf.length() > 14)
						{
							if (lbuf.substr(0, 10) == "[download]")
							{
								std::string dirtyDigits = lbuf.substr(11, 3);
								std::stringstream ss;
								for (std::size_t j = 0; j < dirtyDigits.length(); j++)
								{
									if ((dirtyDigits[j] >= '0') && (dirtyDigits[j] <= '9')) ss << dirtyDigits[j];
								}
								if (ss.str().length() > 0)
								{
									int newPercentage = std::stoi(ss.str());
									unfinishedCache[i].download_progress = newPercentage;
								}
							}
						}
					}
				}
				ifs.close();
			}
		}
	}

	lastProgressCheck = time(0);
	return;
}

std::size_t DownloadManager::GetQueueLength()
{
	std::size_t counter = 0;
	for (std::size_t i = 0; i < unfinishedCache.size(); i++)
	{
		if (unfinishedCache[i].status == DOWNLOAD_STATUS::QUEUED) counter++;
	}
	return counter;
}

JsonArray DownloadManager::GetAlltimeCacheAsJson(time_t max_age, std::size_t max_num)
{
	JsonArray arr;
	for (std::size_t i = 0; i < unfinishedCache.size(); i++)
	{
		arr += unfinishedCache[i].GetAsJson();
	}

	arr.Merge(saveFileCache);
	arr.Sort("queued_timestamp", JSON_ARRAY_SORT_MODE::NUM_DESC);

	// Both limits are inifnite. Just return arr as is
	if ((max_age == -1) && (max_num == (std::size_t)-1)) return arr;

	JsonArray cutArr;
	// If max_num is -1 (would mean inifnite) it would underflow to size_t::max
	for (std::size_t i = 0; ((i < arr.Size()) && (cutArr.Size() < max_num)); i++)
	{
		// If max_age is >= 0, we have to check against the max age
		if (max_age >= 0)
		{
			if (arr[i].AsJson.DoesExist("queued_timestamp"))
			{
				if (arr[i]["queued_timestamp"].GetDataType() == JDType::INT)
				{
					if ((time(0) - arr[i]["queued_timestamp"].AsInt) <= max_age)
					{
						cutArr += arr[i];
					}
				}
			}
		}
		else // If not, just insert it
		{
			cutArr += arr[i];
		}
	}

	return cutArr;
}

bool DownloadManager::DoesTubioIDExist(std::string tubioId)
{
	for (std::size_t i = 0; i < unfinishedCache.size(); i++)
	{
		if (unfinishedCache[i].tubio_id == tubioId) return true;
	}
	for (std::size_t i = 0; i < saveFileCache_Atomic.size(); i++)
	{
		if (saveFileCache_Atomic[i].tubio_id == tubioId) return true;
	}

	return false;
}

DownloadEntry& DownloadManager::GetDownloadEntryByTubioID(std::string tubioId)
{
	for (std::size_t i = 0; i < unfinishedCache.size(); i++)
	{
		if (unfinishedCache[i].tubio_id == tubioId) return unfinishedCache[i];
	}
	for (std::size_t i = 0; i < saveFileCache_Atomic.size(); i++)
	{
		if (saveFileCache_Atomic[i].tubio_id == tubioId) return saveFileCache_Atomic[i];
	}

	throw std::exception();
	std::terminate();
}


bool DownloadManager::ClearDownloadCache()
{
	if (GetNumActiveDownloads() == 0)
	{
		log->cout << "Clearing download cache...";
		log->Flush();

		if (FileSystem::ExistsDirectory(XGConfig::downloader.cachedir))
		{
			FileSystem::DeleteDirectory(XGConfig::downloader.cachedir);
			FileSystem::CreateDirectoryIfNotExists(XGConfig::downloader.cachedir + "/metadata");
			FileSystem::CreateDirectoryIfNotExists(XGConfig::downloader.cachedir + "/download");
			FileSystem::CreateDirectoryIfNotExists(XGConfig::downloader.cachedir + "/dlprogbuf");
			unfinishedCache.clear();
			saveFileCache.Clear();
			saveFileCache_Atomic.clear();
		}

		return true;
	}

	log->cout << "Download cache will be cleared as soon as possible...";
	log->Flush();
	shouldClearCacheASAP = true;
	return false;
}

bool DownloadManager::RemoveFromCacheByID(std::string id)
{
	std::string filePath = "";
	bool wasFinished = false;
	for (std::size_t i = 0; i < unfinishedCache.size(); i++)
	{
		if (unfinishedCache[i].tubio_id == id)
		{
			filePath = unfinishedCache[i].downloaded_filename;
			// Edgecase
			wasFinished = unfinishedCache[i].status == DOWNLOAD_STATUS::FINISHED;
			unfinishedCache.erase(unfinishedCache.begin() + i);
			break;
		}
	}
	// It finds it either in the lower two for loops, or in the first one.
	for (std::size_t i = 0; i < saveFileCache.Size(); i++)
	{
		if ((saveFileCache[i].GetDataType() == JDType::JSON) &&
			(saveFileCache[i].AsJson.DoesExist("tubio_id")) &&
			(saveFileCache[i]["tubio_id"].GetDataType() == JDType::STRING))
		{
			if (saveFileCache[i]["tubio_id"].AsString == id)
			{
				saveFileCache.RemoveAt(i);
				break;
			}
		}
	}
	for (std::size_t i = 0; i < saveFileCache_Atomic.size(); i++)
	{
		if (saveFileCache_Atomic[i].tubio_id == id)
		{
			filePath = saveFileCache_Atomic[i].downloaded_filename;
			wasFinished = saveFileCache_Atomic[i].status == DOWNLOAD_STATUS::FINISHED;
			saveFileCache_Atomic.erase(saveFileCache_Atomic.begin() + i);
			break;
		}
	}

	if ((wasFinished) && (FileSystem::Exists(filePath)))
	{
		FileSystem::Delete(filePath);
	}

	// Only save, if we can save immediately
	if (downloadThreads.size() == 0) Save();

	return false;
}

void DownloadManager::Save()
{
	log->cout << "Saving...";
	log->Flush();

	if (downloadThreads.size() > 0)
	{
		log->cout << "Waiting for active download threads to finish before saving...";
		log->Flush();

		for (std::size_t i = 0; i < downloadThreads.size(); i++)
		{
			downloadThreads[i]->join();
			delete downloadThreads[i];
			downloadThreads[i] = nullptr;
		}
		downloadThreads.clear();
		log->cout << "All threads have finished. Now saving...";
		log->Flush();
	}

	// Filecache has contain all and only nonsaved entries.
	for (long long int i = unfinishedCache.size() - 1; i >= 0; i--)
	{
		if (unfinishedCache[i].status == DOWNLOAD_STATUS::FINISHED)
		{
			saveFileCache += unfinishedCache[i].GetAsJson();
			unfinishedCache.erase(unfinishedCache.begin() + i);
		}
	}

	Json j(saveFileCache);
	if (!FileSystem::WriteFile(XGConfig::downloader.cachedir + "/index.json", j.Render()))
	{
		log->cout << log->Err() << "Unable to save download cache index file!";
		log->Flush();
	}
	saveFileCache_Atomic = ParseJsonArrayToEntries(saveFileCache);

	shouldSave = false;

	log->cout << "Saved!";
	log->Flush();

	return;
}

void DownloadManager::Load()
{
	// No file = nothing to load
	if (!FileSystem::Exists(XGConfig::downloader.cachedir + "/index.json"))
	{
		log->cout << "Did not load download cache, because \"" << XGConfig::downloader.cachedir << "/index.json"
			<< "\" was not found.";
		log->Flush();
		return;
	}

	std::string fileContent = FileSystem::ReadFile(XGConfig::downloader.cachedir + "/index.json");

	if (IsJsonValid(fileContent))
	{
		Json j;
		j.Parse(fileContent);
		
		if (j.GetDataType() == JDType::ARRAY)
		{
			saveFileCache.CloneFrom(j.AsArray);
			saveFileCache_Atomic = ParseJsonArrayToEntries(saveFileCache);
		}
		else
		{
			log->cout << log->Err() << "Unable to parse download cache index file! Not json-type array!";
			log->Flush();
		}
	}
	else
	{
		log->cout << log->Err() << "Unable to parse download cache index file! Invalid json!";
		log->Flush();
	}

	return;
}

std::vector<DownloadEntry> DownloadManager::ParseJsonArrayToEntries(const JasonPP::JsonArray& arr)
{
	std::vector<DownloadEntry> entries;

	for (std::size_t i = 0; i < arr.Size(); i++)
	{
		JsonBlock iter = arr[i].AsJson;
		DownloadEntry newEntry;
		newEntry.download_progress = 100;
		newEntry.status = DOWNLOAD_STATUS::FINISHED; // All saved entries are finished...

		if ((iter.DoesExist("title")) && (iter["title"].GetDataType() == JDType::STRING))
		{
			newEntry.title = iter["title"].AsString;
		}

		if ((iter.DoesExist("description")) && (iter["description"].GetDataType() == JDType::STRING))
		{
			newEntry.description = iter["description"].AsString;
		}

		if ((iter.DoesExist("uploader")) && (iter["uploader"].GetDataType() == JDType::STRING))
		{
			newEntry.uploader = iter["uploader"].AsString;
		}

		if ((iter.DoesExist("duration")) && (iter["duration"].GetDataType() == JDType::INT))
		{
			newEntry.duration = (uint32_t)iter["duration"].AsInt;
		}

		if ((iter.DoesExist("tubio_id")) && (iter["tubio_id"].GetDataType() == JDType::STRING))
		{
			newEntry.tubio_id = iter["tubio_id"].AsString;
		}

		if ((iter.DoesExist("webpage_url")) && (iter["webpage_url"].GetDataType() == JDType::STRING))
		{
			newEntry.webpage_url = iter["webpage_url"].AsString;
		}

		if ((iter.DoesExist("thumbnail_url")) && (iter["thumbnail_url"].GetDataType() == JDType::STRING))
		{
			newEntry.thumbnail_url = iter["thumbnail_url"].AsString;
		}

		if ((iter.DoesExist("download_url")) && (iter["download_url"].GetDataType() == JDType::STRING))
		{
			newEntry.download_url = iter["download_url"].AsString;
		}

		if ((iter.DoesExist("downloaded_filename")) && (iter["downloaded_filename"].GetDataType() == JDType::STRING))
		{
			newEntry.downloaded_filename = iter["downloaded_filename"].AsString;
		}

		if ((iter.DoesExist("queued_timestamp")) && (iter["queued_timestamp"].GetDataType() == JDType::INT))
		{
			newEntry.queued_timestamp = iter["queued_timestamp"].AsInt;
		}

		if ((iter.DoesExist("mode")) && (iter["mode"].GetDataType() == JDType::STRING))
		{
			std::string cachedStrMode = iter["mode"];
			if (cachedStrMode == "video") newEntry.mode = DOWNLOAD_MODE::VIDEO;
			else if (cachedStrMode == "audio") newEntry.mode = DOWNLOAD_MODE::AUDIO;
			else newEntry.mode = DOWNLOAD_MODE::VIDEO;
		}

		entries.push_back(newEntry);
	}

	return entries;
}

void DownloadManager::FetchInformation(std::string url, std::string tubId)
{
	std::stringstream ss;
	ss << "youtube-dl --skip-download --no-warnings --no-call-home --no-playlist --socket-timeout 5 --dump-json \"" << url << "\" > \"" << XGConfig::downloader.cachedir << "/metadata/" << tubId << ".json" << "\"";
	system(ss.str().c_str());
	return;
}

std::string DownloadManager::CreateNewTubioID()
{
	bool isIdUnique = false;
	std::size_t counter = 0;
	std::string newId;

	while (!isIdUnique)
	{
		if (counter > 100000000) throw std::exception();

		newId = Internal::Helpers::Base10_2_X(time(0), "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");

		isIdUnique = true;
		if (unfinishedCache.size() > 0)
		{
			isIdUnique = unfinishedCache[unfinishedCache.size() - 1].tubio_id != newId;
		}

		counter++;
	}

	return newId;
}

void DownloadManager::WarnIfMissingDependenciesWIN()
{
#ifdef _WIN
	if (!FileSystem::Exists("youtube-dl.exe"))
	{
		log->cout << log->Warn() << "Dependency youtube-dl.exe missing! Try updating it! (\"request\": \"update_dep_youtubedl\"). "
				  << "Dependencies HAVE to lie in Tubios working directory! (where the config.json is)";
		log->Flush();
	}
	if (!FileSystem::Exists("ffmpeg.exe"))
	{
		log->cout << log->Warn() << "Dependency ffmpeg.exe missing! (Get it here: https://www.gyan.dev/ffmpeg/builds/). "
			<< "Dependencies HAVE to lie in Tubios working directory! (where the config.json is)";
		log->Flush();
	}
	if (!FileSystem::Exists("ffplay.exe"))
	{
		log->cout << log->Warn() << "Dependency ffplay.exe missing! (Get it here: https://www.gyan.dev/ffmpeg/builds/). "
			<< "Dependencies HAVE to lie in Tubios working directory! (where the config.json is)";
		log->Flush();
	}
	if (!FileSystem::Exists("ffprobe.exe"))
	{
		log->cout << log->Warn() << "Dependency ffprobe.exe missing! (Get it here: https://www.gyan.dev/ffmpeg/builds/). "
			<< "Dependencies HAVE to lie in Tubios working directory! (where the config.json is)";
		log->Flush();
	}
#endif
	return;
}

std::size_t DownloadManager::GetNumActiveDownloads()
{
	std::size_t counter = 0;
	for (std::size_t i = 0; i < unfinishedCache.size(); i++)
	{
		if (unfinishedCache[i].status == DOWNLOAD_STATUS::DOWNLOADING) counter++;
	}
	return counter;
}

void DownloadManager::OnExit()
{
	Save();

	// Clear dlprogbuf directory.
	if (FileSystem::ExistsDirectory(XGConfig::downloader.cachedir + "/dlprogbuf"))
	{
		FileSystem::DeleteDirectory(XGConfig::downloader.cachedir + "/dlprogbuf");
	}
	
	return;
}

void DownloadManager::PostExit()
{
	delete log;

	log = nullptr;

	return;
}


std::vector<DownloadEntry> DownloadManager::unfinishedCache;
std::vector<std::thread*> DownloadManager::downloadThreads;
JasonPP::JsonArray DownloadManager::saveFileCache;
std::vector<DownloadEntry> DownloadManager::saveFileCache_Atomic;
::Logging::Logger* DownloadManager::log;
time_t DownloadManager::lastProgressCheck = 0;
bool DownloadManager::shouldSave = false;
bool DownloadManager::shouldClearCacheASAP = false;



Downloader::DownloadEntry::DownloadEntry()
{
	// Some initial values
	title = "";
	description = "";
	uploader = "";
	duration = -1;
	tubio_id = "";
	webpage_url = "";
	thumbnail_url = "";
	downloaded_filename = "";
	download_url = "";
	status = DOWNLOAD_STATUS::QUEUED;
	mode = DOWNLOAD_MODE::AUDIO;
	download_progress = 0;
	queued_timestamp = 0;

	return;
}

JsonBlock DownloadEntry::GetAsJson()
{
	JsonBlock jb;
	jb.Set(Ele("title",title));
	jb.Set(Ele("description", description));
	jb.Set(Ele("uploader", uploader));
	jb.Set(Ele("duration", duration));
	jb.Set(Ele("tubio_id", tubio_id));
	jb.Set(Ele("webpage_url", webpage_url));
	jb.Set(Ele("thumbnail_url", thumbnail_url));
	jb.Set(Ele("download_progress", download_progress));
	jb.Set(Ele("downloaded_filename", downloaded_filename));
	jb.Set(Ele("download_url", download_url));
	jb.Set(Ele("queued_timestamp", (long long int)queued_timestamp));

	switch (mode)
	{
	case DOWNLOAD_MODE::VIDEO:
		jb.Set(Ele("mode", "video"));
		break;
	case DOWNLOAD_MODE::AUDIO:
		jb.Set(Ele("mode", "audio"));
		break;
	}

	switch (status)
	{
	case DOWNLOAD_STATUS::QUEUED:
		jb.Set(Ele("status", "queued"));
		break;
	case DOWNLOAD_STATUS::DOWNLOADING:
		jb.Set(Ele("status", "downloading"));
		break;
	case DOWNLOAD_STATUS::FINISHED:
		jb.Set(Ele("status", "finished"));
		break;
	case DOWNLOAD_STATUS::FAILED:
		jb.Set(Ele("status", "failed"));
		break;
	}

	return jb;
}
