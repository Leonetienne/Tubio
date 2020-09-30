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
    update(context, instance) {
        axios.post("/api", {
            request: "fetch_session_logs"
        })
        .then(function(response) {
            if (response.data.status === "OK") {
                instance.$store.commit("logs/update", response.data.logs);
            }
        });
    },
};
