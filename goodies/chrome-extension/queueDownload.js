function QueueDownload(url, mode, quality, callback) {

    console.log("Queuing '" + url + "'...");

    axios.post("http://tub.io/api", {
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

      return;
}
