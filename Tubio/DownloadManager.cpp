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
	newDownload.download_url = "/download/" + newDownload.tubio_id;

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
				newDownload.title = j["title"];
			}

			if ((j.AsJson.DoesExist("description")) && (j.AsJson["description"].GetDataType() == JDType::STRING))
			{
				newDownload.description = j["description"];
			}

			if ((j.AsJson.DoesExist("uploader")) && (j.AsJson["uploader"].GetDataType() == JDType::STRING))
			{
				newDownload.uploader = j["uploader"];
			}

			if ((j.AsJson.DoesExist("duration")) && (j.AsJson["duration"].GetDataType() == JDType::INT))
			{
				newDownload.duration = j["duration"];
			}

			if ((j.AsJson.DoesExist("webpage_url")) && (j.AsJson["webpage_url"].GetDataType() == JDType::STRING))
			{
				newDownload.webpage_url = j["webpage_url"];
			}

			if ((j.AsJson.DoesExist("thumbnails")) && (j.AsJson["thumbnails"].GetDataType() == JDType::ARRAY))
			{
				JsonArray& thumbnails = j.AsJson["thumbnails"].AsArray;
				if (thumbnails.Size() > 0)
				{
					if (thumbnails.Size() > 1)
					{
						// If we have more than one thumbnail to choose from, choose the second-highes quality.
						newDownload.thumbnail_url = thumbnails[thumbnails.Size() - 2]["url"];
					}
					else
					{
						newDownload.thumbnail_url = thumbnails[thumbnails.Size() - 1]["url"];
					}
				}
			}
		}
	}

	queue.push_back(newDownload);

	return tubioId;
}

void DownloadManager::Update()
{
	if (shouldSave) Save();

	std::size_t cachedNumActiveDownloads = GetNumActiveDownloads();

	// Queue next download, if available
	if (cachedNumActiveDownloads < XGConfig::downloader.num_threads)
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

	for (std::size_t i = 0; i < queue.size(); i++)
	{
		if (queue[i].status == DOWNLOAD_STATUS::QUEUED)
		{
			next = &queue[i];
			break;
		}
	}
	next->status = DOWNLOAD_STATUS::DOWNLOADING;

	std::thread* downloadThread = new std::thread([=]() {
		DownloadEntry* entry = next;

		std::stringstream ss;
		if (entry->mode == DOWNLOAD_MODE::VIDEO)
		{
			std::string ytdl_call_video_base = 
				"youtube-dl --newline --no-call-home --no-playlist --no-part --no-warnings --limit-rate $$DL_RATE"
				" --no-mtime --no-cache-dir --recode-video mp4 --format \"bestvideo[ext=mp4]+bestaudio/best[ext=mp4]/best\""
				" --merge-output-format mp4 -o \"$$DL_FILE\" $$DL_URL > \"$$DL_PROG_BUF_FILE\"";

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
				"youtube-dl --newline --no-call-home --no-playlist --no-part --no-warnings --limit-rate $$DL_RATE"
				" --no-mtime --no-cache-dir --audio-format mp3 --audio-quality 0 --extract-audio -o \"$$DL_FILE\""
				" $$DL_URL > \"$$DL_PROG_BUF_FILE\"";

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
	for (std::size_t i = 0; i < queue.size(); i++)
	{
		if (queue[i].status == DOWNLOAD_STATUS::DOWNLOADING)
		{
			std::string filePath = XGConfig::downloader.cachedir + "/dlprogbuf/" + queue[i].tubio_id + ".buf";
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
									queue[i].download_progress = newPercentage;

									//if (newPercentage == 100) queue[i].status = DOWNLOAD_STATUS::FINISHED;
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
	for (std::size_t i = 0; i < queue.size(); i++)
	{
		if (queue[i].status == DOWNLOAD_STATUS::QUEUED) counter++;
	}
	return counter;
}

JsonArray DownloadManager::GetQueueAsJson()
{
	JsonArray arr;
	for (std::size_t i = 0; i < queue.size(); i++)
	{
		arr += queue[i].GetAsJson();
	}

	return arr;
}

bool DownloadManager::DoesTubioIDExist(std::string tubioId)
{
	for (std::size_t i = 0; i < queue.size(); i++)
	{
		if (queue[i].tubio_id == tubioId) return true;
	}
	return false;
}

DownloadEntry& DownloadManager::GetDownloadEntryByTubioID(std::string tubioId)
{
	for (std::size_t i = 0; i < queue.size(); i++)
	{
		if (queue[i].tubio_id == tubioId) return queue[i];
	}
	throw std::exception("TubioID not found!");
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
			queue.clear();
		}

		return true;
	}

	log->cout << "Download cache will be cleared as soon as possible...";
	log->Flush();
	shouldClearCacheASAP = true;
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

	JsonArray arr;
	for (std::size_t i = 0; i < queue.size(); i++)
	{
		if (queue[i].status == DOWNLOAD_STATUS::FINISHED)
		{
			arr += queue[i].GetAsJson();
		}
	}

	Json j(arr);
	if (!FileSystem::WriteFile(XGConfig::downloader.cachedir + "/index.json", j.Render()))
	{
		log->cout << log->Err() << "Unable to save download cache index file!";
		log->Flush();
	}

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
			const JsonArray& cachedArr = j.AsArray;

			for (std::size_t i = 0; i < cachedArr.Size(); i++)
			{
				JsonBlock iter = cachedArr[i].AsJson;
				DownloadEntry newEntry;
				newEntry.download_progress = 100;
				newEntry.status = DOWNLOAD_STATUS::FINISHED; // All saved entries are finished...

				if ((iter.DoesExist("title")) && (iter["title"].GetDataType() == JDType::STRING))
				{
					newEntry.title = iter["title"];
				}

				if ((iter.DoesExist("description")) && (iter["description"].GetDataType() == JDType::STRING))
				{
					newEntry.description = iter["description"];
				}

				if ((iter.DoesExist("uploader")) && (iter["uploader"].GetDataType() == JDType::STRING))
				{
					newEntry.uploader = iter["uploader"];
				}

				if ((iter.DoesExist("duration")) && (iter["duration"].GetDataType() == JDType::INT))
				{
					newEntry.duration = iter["duration"];
				}

				if ((iter.DoesExist("tubio_id")) && (iter["tubio_id"].GetDataType() == JDType::STRING))
				{
					newEntry.tubio_id = iter["tubio_id"];
				}

				if ((iter.DoesExist("webpage_url")) && (iter["webpage_url"].GetDataType() == JDType::STRING))
				{
					newEntry.webpage_url = iter["webpage_url"];
				}

				if ((iter.DoesExist("thumbnail_url")) && (iter["thumbnail_url"].GetDataType() == JDType::STRING))
				{
					newEntry.thumbnail_url = iter["thumbnail_url"];
				}

				if ((iter.DoesExist("download_url")) && (iter["download_url"].GetDataType() == JDType::STRING))
				{
					newEntry.download_url = iter["download_url"];
				}

				if ((iter.DoesExist("downloaded_filename")) && (iter["downloaded_filename"].GetDataType() == JDType::STRING))
				{
					newEntry.downloaded_filename = iter["downloaded_filename"];
				}

				if ((iter.DoesExist("mode")) && (iter["mode"].GetDataType() == JDType::STRING))
				{
					std::string cachedStrMode = iter["mode"];
					if (cachedStrMode == "video") newEntry.mode = DOWNLOAD_MODE::VIDEO;
					else if (cachedStrMode == "audio") newEntry.mode = DOWNLOAD_MODE::AUDIO;
					else newEntry.mode = DOWNLOAD_MODE::VIDEO;
				}

				queue.push_back(newEntry);
			}
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

void DownloadManager::FetchInformation(std::string url, std::string tubId)
{
	std::stringstream ss;
	ss << "youtube-dl.exe --skip-download --dump-json " << url << " > \"" << XGConfig::downloader.cachedir << "/metadata/" << tubId << ".json" << "\"" << std::endl;
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
		if (counter > 100000000) throw std::exception("Tubio download id generator timeout");

		newId = Internal::Helpers::Base10_2_X(time(0), "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");

		isIdUnique = true;
		for (std::size_t i = 0; i < queue.size(); i++)
		{
			if (queue[i].tubio_id == newId) isIdUnique = false;
		}
		counter++;
	}

	return newId;
}

std::size_t DownloadManager::GetNumActiveDownloads()
{
	std::size_t counter = 0;
	for (std::size_t i = 0; i < queue.size(); i++)
	{
		if (queue[i].status == DOWNLOAD_STATUS::DOWNLOADING) counter++;
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


std::vector<DownloadEntry> DownloadManager::queue;
std::vector<std::thread*> DownloadManager::downloadThreads;
::Logging::Logger* DownloadManager::log;
time_t DownloadManager::lastProgressCheck = 0;
bool DownloadManager::shouldSave = false;
bool DownloadManager::shouldClearCacheASAP = false;



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
