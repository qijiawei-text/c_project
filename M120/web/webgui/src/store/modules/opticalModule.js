
const opticalModule = {
    state: {
        opticalModules: {}
    },

    mutations: {
        SET_OPTICALMODULES: (state, opticalmodules) => {
            state.opticalmodules = opticalmodules
            console.log('mutations save state.opticalmodules', opticalmodules)
        },
    },
    actions: {
        setOpticalModules({ commit }, opticalmodules) {
            commit('SET_OPTICALMODULES', opticalmodules)
            },
    },
}
export default opticalModule


