import axios from 'axios';

export const state = () => ({
    logs: []
});

export const mutations = {
    update(state, data) {
        state.logs = data;
    },
};

export const actions = {
    update(context) {
        axios.post("/api", {
            request: "fetch_session_logs"
        })
        .then(function(response) {
            if (response.data.status === "OK") {
                context.commit("update", response.data.logs);
            }
        });
    },
};
