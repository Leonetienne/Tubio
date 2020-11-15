#include "Updater.h"

std::string Updater::UpdateYoutubeDL()
{
#ifdef _WIN
    HRESULT res = URLDownloadToFileA(NULL, "https://yt-dl.org/downloads/latest/youtube-dl.exe", "youtube-dl.exe", 0, NULL);
    
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
