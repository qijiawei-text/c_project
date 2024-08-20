
const osc = {
    state: {
        oscs: {}
    },

    mutations: {
        SET_OSCS: (state, oscs) => {
            state.oscs = oscs
            console.log('mutations save state.oscs', oscs)
        },
    },
    actions: {
        setSocs({ commit }, voas) {
            commit('SET_OSCS', voas)
            },
    },
}
export default osc


