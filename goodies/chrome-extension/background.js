function Queue(info) {

    // Select either the current page location, or the target of a href (if the user clicked on one)
    urlToDownload = typeof(info.linkUrl) != 'undefined' ? info.linkUrl : info.pageUrl;

    axios.post("http://tub.io/api", {
        request: "queue_download",
        video_url: urlToDownload,
        mode: "video",
        quality: "best"
      }).then(function(response){
            console.log("Queued successfully...");
        }).catch(function(response){
            console.log("Something went wrong...");
            console.log(response);
      });
}

chrome.contextMenus.create({
    "title" : "Queue in Tubio",
    "contexts": ["all"],
    "type" : "normal",
    "onclick" : Queue
});

