import axios from 'axios';

export const state = () => ({
    serverVersion: -1
});

export const mutations = {
    update(state, data) {
        state.serverVersion = data;
    },
};

export const actions = {
    update(context) {
        axios.post("/api", {
            request: "get_server_version"
        })
        .then(function(response) {
            if (response.data.status === "OK") {
                context.commit("update", response.data.server_version);
            }
        });
    },
};
