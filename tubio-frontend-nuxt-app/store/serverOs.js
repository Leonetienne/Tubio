import axios from 'axios';

export const state = () => ({
    os_name: ""
});

export const mutations = {
    update(state, data) {
        state.os_name = data;
    },
};

export const actions = {
    update(context, instance) {
        axios.post("/api", {
            request: "get_os_name"
        })
        .then(function(response) {
            if (response.data.status === "OK") {
                instance.$store.commit("serverOs/update", response.data.os_name);
            }
        });
    },
};
