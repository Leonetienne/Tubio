$("#button-download").click(function() {

    $("#button-download").addClass("lock");

    // Query active tab in active window...
    chrome.windows.getCurrent(function(w) {
        chrome.tabs.query({
            active: true,
            windowId: w.id
        }, function(foundTabs) {
            if (foundTabs.length > 0) {
                QueueDownload(foundTabs[0].url, "video", "best", function(){
                    window.close();
                });
            } else {
                console.log("Wtf, this should not happen. You don't have ANY open tab?...");
            }
        });
    });

    return;
});
