
const device = {
    state: {
        nodes: {}
    },

    mutations: {
        SET_NODES: (state, nodes) => {
            state.nodes = nodes
            console.log('mutations save state.nodes', nodes)
        },
    },
    actions: {
        setNodes({ commit }, nodes) {
            commit('SET_NODES', nodes)
            },
    },
}
export default device


