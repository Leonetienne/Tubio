function QueueDownload(url, mode, quality, callback) {

    console.log("Queueing '" + url + "'...");
    
    // Fetch tubio address
    chrome.storage.local.get(['tubio_address'], function(result) {
        
        if ((typeof(result.tubio_address) == 'undefined') ||
            (result.tubio_address.length == 0)) {
                alert("Please first set an address to reach Tubio at in the extensions settings! (Click on the extensions icon in the toolbar).");
                return;
        } else {

            tubioUrl = result.tubio_address;
            if (tubioUrl[tubioUrl.length-1] != '/') { // Has the user not appended a /?
                tubioUrl += '/';
            }

            axios.post(tubioUrl + "api", {
                request: "queue_download",
                video_url: url,
                mode: mode,
                quality: quality
            }).then(function(response){
                    console.log("Queued successfully...");
                    if (typeof(callback) != 'undefined') callback(true, response);
                    
                }).catch(function(response){
                    console.log("Something went wrong...");
                    console.log(response);
                    if (typeof(callback) != 'undefined') callback(false, response);
            });
        }
    });


      return;
}
