
const voa = {
    state: {
        voas: {}
    },

    mutations: {
        SET_VOAS: (state, voas) => {
            state.voas = voas
            console.log('mutations save state.voas', voas)
        },
    },
    actions: {
        setVoas({ commit }, voas) {
            commit('SET_VOAS', voas)
        },
    },
}
export default voa


