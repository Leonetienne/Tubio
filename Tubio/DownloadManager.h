#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <ctime>
#include <thread>
#include "FileSystem.h"
#include "XGConfig.h"
#include "Logger.h"

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

	class DownloadEntry
	{
	public:
		std::string title;
		std::string description;
		std::string uploader;
		int duration;
		std::string tubio_id;
		std::string webpage_url;
		std::string thumbnail_url;
		DOWNLOAD_STATUS status;
		DOWNLOAD_MODE mode;
		int download_progress;

		JasonPP::JsonBlock GetAsJson();

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
		static std::string QueueDownload(std::string url, DOWNLOAD_MODE mode);

		/// <summary>
		/// Returns the number of videos queued
		/// </summary>
		/// <returns></returns>
		static std::size_t GetQueueLength();

		static JasonPP::JsonArray GetQueueAsJson();

		/// <summary>
		/// Will delete all cached downloads!
		/// </summary>
		static void ClearDownloadCache();

	private:
		static void Save();
		static void Load();

		static void FetchInformation(std::string url, std::string tubId);
		static std::string CreateNewTubioID();

		static std::size_t GetNumActiveDownloads();

		/// <summary>
		/// Will start a download-thread for the next queue-entry with status "queued"
		/// </summary>
		static void DownloadNext();
		static void UpdateDownloadProgressPercentages();

		static std::vector<DownloadEntry> queue;
		static std::vector<std::thread*> downloadThreads;
		static Logging::Logger* log;
		// This gets set by other threads
		static bool shouldSave;
		static time_t lastProgressCheck;
	};
}