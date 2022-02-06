#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <ctime>
#include <thread>
#include "FileSystem.h"
#include "XGConfig.h"
#include "Logger.h"
#include "LogHistory.h"

namespace Downloader
{
	enum class DOWNLOAD_MODE
	{
		VIDEO,
		AUDIO
	};
	enum class DOWNLOAD_STATUS
	{
		QUEUED,
		DOWNLOADING,
		FINISHED,
		FAILED
	};
	enum class DOWNLOAD_QUALITY
	{
		_BEST,   // best quality
		_1440p,  // 1440p
		_1080p,  // 1080p
		_720p,   // 720p
		_480p,   // 480p
		_360p,   // 360p
		_240p,   // 240p
		_144p,    // 144p
		INVALID
	};

	class DownloadEntry
	{
	public:
		DownloadEntry();
		JasonPP::JsonBlock GetAsJson();

		std::string title;
		std::string description;
		std::string uploader;
		int duration;
		std::string tubio_id;
		std::string webpage_url;
		std::string thumbnail_url;
		std::string downloaded_filename;
		std::string download_url;
		DOWNLOAD_STATUS status;
		DOWNLOAD_MODE mode;
		DOWNLOAD_QUALITY quality;
		int download_progress;
		time_t queued_timestamp;


	};

	class DownloadManager
	{
	public:
		static void PreInit();
		static void Update();
		static void OnExit();
		static void PostExit();

		/// <summary>
		/// Queues a video for download. Returns its tubio download id
		/// </summary>
		/// <param name="url"></param>
		/// <param name="mode">If video or audio</param>
		/// <returns>Tubio download id</returns>
		static std::string QueueDownload(std::string url, DOWNLOAD_MODE mode, DOWNLOAD_QUALITY quality = DOWNLOAD_QUALITY::_BEST);

		/// <summary>
		/// Returns the number of videos queued
		/// </summary>
		/// <returns></returns>
		static std::size_t GetQueueLength();

		/// <summary>
		/// Will return the whole cache in json format
		/// </summary>
		/// /// <param name="max_age">Maximum age of the entry in seconds. -1 = infinite</param>
		/// /// <param name="max_num">Maximum of entries to fetch. -1 = infinite</param>
		/// <returns></returns>
		static JasonPP::JsonArray GetAlltimeCacheAsJson(time_t max_age, std::size_t max_num);

		/// <summary>
		/// Returns whether or not a tubio id exists
		/// </summary>
		/// <param name="tubioId">The id to check</param>
		static bool DoesTubioIDExist(std::string tubioId);

		/// <summary>
		/// Returns a reference to a DownloadEntry by its tubio id
		/// </summary>
		/// <param name="tubioId">The corresponding tubio id</param>
		static DownloadEntry& GetDownloadEntryByTubioID(std::string tubioId);

		/// <summary>
		/// Will delete all cached downloads!
		/// If downloads are currently active, tubio will wait for them to finish and return false!
		/// If no downloads are active it will clear immediately and return true
		/// </summary>
		static bool ClearDownloadCache();

		/// <summary>
		/// Will remove an individual download entry by its tubio id
		/// </summary>
		/// <returns></returns>
		static bool RemoveFromCacheByID(std::string id);

		/// <summary>
		/// Will return a name of a download quality. Like, '1080p' or 'best' for example
		/// </summary>
		/// <param name="quality">The quality to get the name from</param>
		/// <returns>The name of the download quality</returns>
		static std::string DownloadQualityToName(DOWNLOAD_QUALITY quality);

		/// <summary>
		/// Will return a download quality object based on a name, like '1080p' or 'best' for example
		/// </summary>
		/// <param name="qualityName"></param>
		/// <returns></returns>
		static DOWNLOAD_QUALITY GetDownloadQualityByName(const std::string& qualityName);

	private:
		static void Save();
		static void Load();
		static std::vector<DownloadEntry> ParseJsonArrayToEntries(const JasonPP::JsonArray& arr);

		/// <summary>
		/// Will return a yt-dlp quality string based on 'quality'
		/// </summary>
		/// <param name="quality">The download quality to get the parameter from</param>
		/// <returns>The yt-dlp quality parameter</returns>
		static std::string DownloadQualityToStringParams(DOWNLOAD_QUALITY quality);

		/// <summary>
		/// Will fetch metadata of an url
		/// </summary>
		/// <param name="url">Url to fetch from</param>
		/// <param name="tubId">Tubio id to save data to</param>
		static void FetchInformation(std::string url, std::string tubId);

		/// <summary>
		/// Will create an unique tubio id (based on time())
		/// </summary>
		/// <returns>Unique tubio id</returns>
		static std::string CreateNewTubioID();

		/// <summary>
		/// Will check for missing dependencies (windows only) and warn if missing
		/// </summary>
		static void WarnIfMissingDependenciesWIN();

		static std::size_t GetNumActiveDownloads();

		/// <summary>
		/// Will start a download-thread for the next queue-entry with status "queued"
		/// </summary>
		static void DownloadNext();
		static void UpdateDownloadProgressPercentages();

		static std::vector<DownloadEntry> unfinishedCache;
		static std::vector<std::thread*> downloadThreads;
		static JasonPP::JsonArray saveFileCache; // Content of the save-file
		static std::vector<DownloadEntry> saveFileCache_Atomic; // Content of the save-file
		static Logging::Logger* log;
		// This gets set by other threads
		static time_t lastProgressCheck;
		static bool shouldSave;
		static bool shouldClearCacheASAP;
	};
}
