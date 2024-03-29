![Tubio](https://gitea.leonetienne.de/leonetienne/Tubio/raw/branch/master/github-media/teaser.png)

# Tubio
The free, open source video downloader!

No longer do you have to rely on shady websites, shoving tons of ads in your face to fulfil your downloady needs. No longer will you be held back by artificially crippled download speeds, login-/paywalls or even watermarks.

## But what is Tubio?
Tubio in of itself is not a downloader, but a GUI for the widely known, open-source, public-domain cli [yt-dlp](https://github.com/yt-dlp/yt-dlp/). <sup>Thanks for this awesome tool! You guys are heroes! (Obviously, same goes for youtube-dl, which yt-dlp is based on)</sup>
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

![soundcloud](https://gitea.leonetienne.de/leonetienne/Tubio/raw/branch/master/github-media/soundcloud.png)

![reddit](https://gitea.leonetienne.de/leonetienne/Tubio/raw/branch/master/github-media/reddit.png)

![youtube](https://gitea.leonetienne.de/leonetienne/Tubio/raw/branch/master/github-media/youtube.png)

![settings](https://gitea.leonetienne.de/leonetienne/Tubio/raw/branch/master/github-media/settings.png)

![mobile](https://gitea.leonetienne.de/leonetienne/Tubio/raw/branch/master/github-media/mobile.png)

## There's also a Chrome Extension
![extension-popup](https://gitea.leonetienne.de/leonetienne/Tubio/raw/branch/master/github-media/chromeext-popup.png)

![extension-contextmenu](https://gitea.leonetienne.de/leonetienne/Tubio/raw/branch/master/github-media/chromeext-context.png)


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
Tubio hosts a webserver, after all. It is intended to be used for localhost only (since no encryption), but no one is preventing you from unticking `localhost only` under `/settings`. Then you could connect to it via your local IPv4 address (such as `192.168.1.12`) or even over the global WAN! However, regarding WAN, i would **strongly** advise against such a careless setup.

It wouldn't be that complicated to enable TLS. You'd have to install and link `libcryptopp`, obtain a certificate + key, and pass them to the mongoose webserver. But that would break that whole *"compiles without any dependencies thing"*. See [the mongoose docs TLS page](https://mongoose.ws/tutorials/tls/) for instructions. If you implement this cleanly, like with a special make target, a merge request would be greatly appreciated.

!!! IMPORTANT
Tubio does NOT manage sessions or accounts! Everyone using your Tubio instance will see **all your downloads** and vica versa.

If you opt for unleashing Tubio on your LAN, i would **strongly** recommend enabling the whitelist! You can do this either in the `config.json` or in /settings. Either way, it is a json-array of strings which represent IPv4 addresses.

## Setup (Linux)
1) Clone this repository and build it with cmake 
2) Create some folder on your system. This will be the installation folder.
3) In this folder, dump the contents of /Tubio/.
4) Install python3 and then yt-dlp via `pip install yt-dlp`.
5) Launch the tubio executable
6) Enjoy &lt;3

## Frequently Asked Questions
### My downloads hang at 99% or 100%
This happens as there almost always is some post-processing to do after downloading. Like, audio-extracting and/or video recoding. If you're downloading
a video this may take a while, as it may be converting webm to mp4.

### My downloads fail!
First thing to do: Verify that are using the latest version of yt-dlp. The team behind its upstream, youtube-dl, are quite fast adjusting the downloading backend to changes in popular platforms, such as youtube. If your downloads still fail,
verify that yt-dlp is in fact capable of downloading the url you supplied by executing `yt-dlp $url`, replacing `$url` with your url. If this works, you can extract the tubio id of the url you just queued in `/settings`, and then open `<your-tubio-host>/downloadlog/<tubio-id>` to see the output stdout and stderr of the yt-dlp call. Then work from the error messages you see in there.

If it\`s still not working, you\`re out of luck. :(
You may want to check youtube-dl compatible sites.

### I locked myself out by enabling localhost only on another device!
This can only be undone from localhost. Open Tubio via, and this is important, either `localhost` or `127.0.0.1` and untick it again. If you can only ssh into the host, you can edit the `config.json` itself and restart Tubio.

### Does it work on Windows?
Sure it does. You just have to compile it yourself using cmake, and put yt-dlp.exe, ffmpeg.exe, ffplay.exe and ffprobe.exe
in Tubios working directory (installation directory). You could then just launch it at startup.

### Can I use it on my phone?
You bet! That's what I'm mainly using it for :) Read [this](#user-content-can-i-use-tubio-on-multiple-devices).

### Can I use this to host my own downloader website?
On your own risk! Tubio is NOT designed for this! Also do note that tubio does NOT manage sessions or accounts! Everyone accessing this instance can see everyones downloads and access the admin panel! Tubio is really designed for one user!

### XY is not working, you have to fix it now!
I do not. Tubio is a tool I originally made for myself only. I just thought it\`s nice and worth sharing. I will address issues when I have time. Feel free to submit issues and I will have a look when I get to it. :)

### XY is not polished enough!
This is an alpha-version. What did you expect? :D

### Can you please add support for website XY?
Please address the awesome team at youtube-dl, as they make the downloading-backend: [github.com/ytdl-org/youtube-dl/issues](https://github.com/ytdl-org/youtube-dl/issues).
The downloader uses ([yt-dlp](https://github.com/yt-dlp/yt-dlp/)), which is based on youtube-dl.

## Important notice!
I do NOT endorse illegal downloads in any way, shape, or form. Tubio is a tool to download media from legal sources! Use Tubio at your own discretion! Neither do i provide ANY warranty in ANY way, shape, or form!

## License
Tubio is distributed under the GNU General Public License v3.0.
Please read the [license file](https://gitea.leonetienne.de/leonetienne/Tubio/src/branch/master/LICENSE).

