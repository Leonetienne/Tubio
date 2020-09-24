$(".button").mousedown(function(e){
    // Animations
    e.target.classList.add("clicked");
    setTimeout(function(){
        e.target.classList.remove("clicked");
    }, 100);
});

$("#btn_foo").click(function(e){
    // Functionality
    axios.post("/api", {
        request: "foo"
    }).then(function(response){
        // Do something with the servers response
    });
});

$("#btn_killserver").click(function(e){
    // Functionality
    axios.post("/api", {
        request: "kill_yourself"
    }).then(function(response){
        if (response.data.status == "OK") {
            $("#headline").html("Sever's dead");
        }
    });
});
