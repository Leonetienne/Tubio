$("#button--download").click(function() {

    $("#button--download").addClass("lock");

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

chrome.storage.local.get(['tubio_address'], function(result) {
    $("#tubio-address").val(result.tubio_address);
});

$("#button--save-settings").click(function() {
    chrome.storage.local.set({
        tubio_address: $("#tubio-address").val()
    });
});

$("#button--take-me-home").click(function() {
    chrome.storage.local.get(['tubio_address'], function(result) {
        chrome.tabs.create({url: result.tubio_address});
    });
});
