import axios from 'axios';

export const state = () => ({
    config: {}
});

export const mutations = {
    updateGet(state, data) {
        state.config = data;
    },
};

export const actions = {
    updateGet(context, callback) {
        axios.post("/api", {
            request: "update_config"
        })
        .then(function(response) {
            if (response.data.status === "OK") {
                context.commit("updateGet", response.data.config);
                if (typeof(callback) === "function") callback();
            }
        });
    },
    updateSet(context, newConfig) {
        axios.post("/api", {
            request: "update_config",
            config: newConfig
        })
        .then(function(response) {
            if (response.data.status === "OK") {
                // Also update the vuex store...
                context.commit("updateGet", response.data.config);
            }
        });
    },
    resetToDefaults(context) {
        axios.post("/api", {
            request: "reset_config_to_default_values"
        })
        .then(function(response) {
            if (response.data.status === "OK") {
                // Also update the vuex store...
                context.commit("updateGet", response.data.config);
            }
        });
    }
};
