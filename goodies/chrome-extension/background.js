function QueueDownloadByInfo(info, mode, quality) {

    // Select either the current page location, or the target of a href (if the user clicked on one)
    QueueDownload(
        urlToDownload = typeof(info.linkUrl) != 'undefined' ? info.linkUrl : info.pageUrl,
        mode,
        quality
    ); // <- Defined in queueDwonload.js

    return;
}

// Create main context menu entry
chrome.contextMenus.create({
    title: "Tubio",
    contexts:["all"],
    id: "tubio-parent-contextmenu-entry",
});

// Automate creating a lot of entries
function CreateContextMenuOption(optionName, callback) {
    chrome.contextMenus.create({
        title: optionName,
        contexts: ["all"],
        type: "normal",
        parentId: "tubio-parent-contextmenu-entry",
        onclick: callback
    });
}

// Create all download methods
CreateContextMenuOption("Download Video - Best", function(info) {
    QueueDownloadByInfo(info, "video", "best");
});
CreateContextMenuOption("Download Video - 1440p", function(info) {
    QueueDownloadByInfo(info, "video", "1440p");
});
CreateContextMenuOption("Download Video - 1080p", function(info) {
    QueueDownloadByInfo(info, "video", "1080p");
});
CreateContextMenuOption("Download Video - 720p", function(info) {
    QueueDownloadByInfo(info, "video", "720p");
});
CreateContextMenuOption("Download Video - 480p", function(info) {
    QueueDownloadByInfo(info, "video", "480p");
});
CreateContextMenuOption("Download Video - 360p", function(info) {
    QueueDownloadByInfo(info, "video", "360p");
});
CreateContextMenuOption("Download Video - 240p", function(info) {
    QueueDownloadByInfo(info, "video", "240p");
});
CreateContextMenuOption("Download Video - 144p", function(info) {
    QueueDownloadByInfo(info, "video", "144p");
});
CreateContextMenuOption("Download Audio", function(info) {
    QueueDownloadByInfo(info, "audio", "best"); // <- Quality is ignored when downloading audio only 
});
