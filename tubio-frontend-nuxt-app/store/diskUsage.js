import axios from 'axios';

export const state = () => ({
    usage: {}
});

export const mutations = {
    update(state, data) {
        state.usage = data;
    },
};

export const actions = {
    update(context) {
        axios.post("/api", {
            request: "get_disk_usage"
        })
        .then(function(response) {
            if (response.data.status === "OK") {
                context.commit("update", response.data.disk_usage);
            }
        });
    },
};
