import axios from 'axios';

export const state = () => ({
    cache: []
});

export const mutations = {
    update(state, data) {
        state.cache = data;
    },
};

export const actions = {
    update(context) {
        axios.post("/api", {
            request: "fetch_alltime_cache"
        })
        .then(function(response) {
            if (response.data.status === "OK") {
                context.commit("update", response.data.cache);
            }
        });
    },
};
