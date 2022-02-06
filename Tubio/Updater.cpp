#include "Updater.h"

using namespace JasonPP;

std::string Updater::UpdateYtDlp()
{
#ifdef _WIN
    // Fetch rest respone for latest yt-dlp release
    CComPtr<IStream> dlStream;
    HRESULT res = URLOpenBlockingStreamA(
        nullptr,
        "https://api.github.com/repos/yt-dlp/yt-dlp/releases/latest",
        &dlStream,
        0,
        nullptr
    );
    if (FAILED(res))
    {
        return "Error fetching latest yt-dlp release identifier. Error code: 0x" + (JasonPP::Internal::Helpers::Base10_2_X(res, "0123456789abcdef"));
    }

    char buffer[4096];
    std::stringstream restContent;
    do
    {
        DWORD bytesRead = 0;
        res = dlStream->Read(buffer, sizeof(buffer), &bytesRead);

        if (bytesRead)
            restContent.write(buffer, bytesRead);
    } while ((SUCCEEDED(res)) && (res != S_FALSE));

    // Parse response
    Json json;
    try
    {
        json.Parse(restContent.str());
    }
    catch (JsonException& e)
    {
        return "Error parsing the json githubs rest api returned, whilst trying to update yt-dlp.";
    }
    
    // Look for the asset in the release that's named "yt-dlp.exe"
    std::string downloadUrlLatestExe = "";
    try
    {
        const JsonArray assetsArr = json.AsJson["assets"].AsArray;

        for (std::size_t i = 0; i < assetsArr.Size(); i++)
        {
            if (assetsArr[i].AsJson["name"] == "yt-dlp.exe")
                downloadUrlLatestExe = assetsArr[i].AsJson["browser_download_url"];
        }
    }
    catch (JsonException& e)
    {
        return "Error whilst trying to access the json key assets[n][\"name\"/\"browser_download_url\"] whilst trying to update yt-dlp.";
    }

    if (downloadUrlLatestExe == "")
    {
        return "Error: No suitable asset found in latest release. Looking for name \"yt-dlp.exe\".";
    }


    // Download the latest yt-dlp.exe
    res = URLDownloadToFileA(NULL, downloadUrlLatestExe.c_str(), "yt-dlp.exe", 0, NULL);
    
    if (SUCCEEDED(res))
    {
        return "OK";
    }
    else
    {
        return "error code: 0x" + (JasonPP::Internal::Helpers::Base10_2_X(res, "0123456789abcdef"));
    }
#else
    return "not implemented";
#endif
}
