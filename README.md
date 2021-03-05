![Tubio](https://raw.githubusercontent.com/Leonetienne/Tubio/master/github-media/teaser.png)

# Tubio
The free, open source video downloader!

No longer do you have to rely on shady websites, shoving tons of ads in your face to fulfil your downloady needs. No longer will you be held back by artificially crippled download speeds, login-/paywalls or even watermarks.

## But what is Tubio?
Tubio in of itself is not a downloader, but a GUI for the widely known, open-source, public-domain cli [youtube-dl](http://youtube-dl.org/). <sup>Thanks for this awesome tool! You guys are heroes!</sup>
The goal of Tubio is to make this awesome software more accessible. Not everyone knows how to use the command line!

## But, how does it work?
It\`s quite easy! Make sure the Tubio service is running in the background, navigate to http://localhost, paste in your video/music url and chances are that it will work!  

This is because the set of supported websites is **extremely** large! See here: [supportedsites.md](https://github.com/blackjack4494/yt-dlc/blob/master/docs/supportedsites.md)

When your download finished, just click "download" and have fun!

## No, i mean on a more technical level
The backend is a C++ webserver, powered by [casenta/mongoose](https://github.com/cesanta/mongoose).<sup>Thanks, you guys are awesome!</sup>
The connection is **not** encrypted, but that\`s okay because it is intended for localhost only. Mongoose does support ssl, so you can always add it, if you fancy it.

The frontend is a nuxt.js web application.

## But how does it look?
Have a sneak peak!

![soundcloud](https://raw.githubusercontent.com/Leonetienne/Tubio/master/github-media/soundcloud.png)

![reddit](https://raw.githubusercontent.com/Leonetienne/Tubio/master/github-media/reddit.png)

![youtube](https://raw.githubusercontent.com/Leonetienne/Tubio/master/github-media/youtube.png)

![settings](https://raw.githubusercontent.com/Leonetienne/Tubio/master/github-media/settings.png)

![mobile](https://raw.githubusercontent.com/Leonetienne/Tubio/master/github-media/mobile.png)

## How can i configure it?
See this json file (config.json).
Most of these values are configurable in the web-interface, but not all, such as the port.
```json
{
  "access": {
    "enable_whitelist": true,
    "only_allow_localhost": false,
    "whitelist": [
      "127.0.0.1",
      "192.168.1.12",
      "192.168.1.14",
      "192.168.1.10"
    ]
  },
  "downloader": {
    "cachedir": "dlcache",
    "max_dlrate_per_thread": "100M",
    "num_threads": 10
  },
  "general": {
    "show_console": true
  },
  "httpServer": {
    "polling_rate": 100,
    "port": "80",
    "rootdir": "./frontend"
  },
  "logging": {
    "autosave_interval": 20,
    "logfile_json": "log.json",
    "logfile_text": "log.txt"
  }
}
```

## Can i use Tubio on multiple devices?
Tubio is a webserver, after all. It is intended to be used for localhost only (since no encryption), but no one is preventing you from unticking `localhost only` under /settings.

Then you could connect to it via your local IPv4 address (such as `192.168.1.12`) or even over the global WAN! However, regarding WAN, i would **strongly** advise against such a careless setup.

!!! IMPORTANT
Tubio does NOT manage sessions or accounts! Everyone using your Tubio instance will see **all your downloads** and vica versa.

If you opt for unleashing Tubio on your LAN, i would **strongly** recommend enabling the whitelist! You can do this either in the `config.json` or in /settings. Either way, it is a json-array of strings which represent IPv4 addresses.

## Setup (Windows)
#### Install youtube-dl.exe:
1) Go here: [click](http://ytdl-org.github.io/youtube-dl/download.html).
2) From there, download Microsoft Visual C++ 2010 Redistributable Package (x86) and install it.
3) Go there: [click](https://www.gyan.dev/ffmpeg/builds/).
4) Download the ffmpeg-release-full.7z. At the time of writing this, the direct link is [this](https://www.gyan.dev/ffmpeg/builds/ffmpeg-release-full.7z). Save that for later.
5) Download the latest Tubio build from [here](https://github.com/Leonetienne/Tubio/releases).
6) Create some folder on your pc. This will be the installation folder.
7) Dump in the contents of the Tubio build you just downloaded.
8) Dump in the contents of `ffmpeg-<some-numbers>/bin/` in ffmpeg-release-full.7z into that same installation directory. It should be three .exe files. They HAVE to be in the same directory as `Tubio.exe`!
9) Launch Tubio.exe
10) Navigate to `http://localhost/settings` and click "Update ytdl", as the version Tubio shipped with might be out of date.
11) &lt;Optional&gt;: Set up `Tubio.exe` to launch with windows to have it\`s service always at hand.
12) Enjoy &lt;3

## Frequently Asked Questions
#### My downloads fail!
First thing to do: Navigate to /settings and click "Update ytdl". This will update your local instance of [youtube-dl](http://youtube-dl.org/).  Check the logs to see if it worked. If not, restart Tubio and try again.

If it\`s still not working, you are most likely trying to download a video from a playlist with a weird url. Try a different one (The one from the share button, the one from right-clicking the video or the one from the url-bar in your browser).

If it\`s still not working, you\`re out of luck. :(

#### I locked myself out by enabling localhost only on another device!
This can only be undone from the host. Open Tubio via, and this is important, either `localhost` or `127.0.0.1` and untick it again. If you can only ssh into the host, you can edit the `config.json` itself and restart Tubio.

#### Does it work on linux?
Well, technically, yes.  You would have to install youtube-dl and the ffmpeg suite yourself, and compile Tubio yourself though. I have not tried it yet, but it should work.

#### Can i use it on my phone?
Sure. Read [this](#user-content-can-i-use-tubio-on-multiple-devices).

#### Can i use this to host my own downloader website?
On your own risk! Tubio is NOT designed for this! Also do note that tubio does NOT manage sessions or accounts! Everyone accessing this instance can see everyones downloads and access the admin panel! Tubio is really designed for one user!

#### XY is not working, you have to fix it now!
I do not. Tubio is a tool i originally made for myself only. I just thought it\`s nice and worth sharing. I will address issues when i have time. Feel free to submit issues and i will have a look when i get to it. :)

#### XY is not polished enough!
This is an alpha-version. What did you expect? :D

#### Can you please add support for website XY?
Please address the awesome team at youtube-dl, as they made the downloading-end: [github.com/ytdl-org/youtube-dl/issues](https://github.com/ytdl-org/youtube-dl/issues)

## Important notice!
I do NOT endorse illegal downloads in any way, shape, or form. Tubio is a tool to download media from legal sources! Use Tubio at your own discretion! Neither do i provide ANY warranty in ANY way, shape, or form!

## License
Tubio is distributed under the GNU General Public License v3.0.
Please read the [license file](https://github.com/Leonetienne/Tubio/blob/master/license.txt).