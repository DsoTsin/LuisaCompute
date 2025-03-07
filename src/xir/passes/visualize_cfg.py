import graphviz
from sys import argv
import json


def parse_control_flow_graphs_from_xir(xir_file):
    with open(xir_file, 'r') as f:
        prefix = "// CFG = "
        lines = [x[len(prefix):] for line in f.readlines() if (x := line.strip()) and x.startswith(prefix)]
    return [json.loads(line) for line in lines]


def normalize_node_name(node):
    return node.replace("%", "block.")


def visualize_control_flow_graph(subgraph, graph_desc):
    nodes = graph_desc['nodes']
    edges = graph_desc['edges']
    dom_edges = graph_desc['dominance_tree']
    frontiers = graph_desc['dominance_frontiers']
    terminators = graph_desc['terminators']
    merges = graph_desc['merges']
    for node in nodes:
        subgraph.node(node, label=f"{normalize_node_name(node)}\n[{terminators[node]}]")
    for source, targets in edges.items():
        for target in targets:
            subgraph.edge(source, target)
    for source, targets in dom_edges.items():
        for target in targets:
            subgraph.edge(source, target, color='red')
    for source, targets in frontiers.items():
        for target in targets:
            subgraph.edge(source, target, color='orange', style='dashed')
    for source, target in merges.items():
        subgraph.edge(source, target, style='dashed')


if __name__ == '__main__':
    if len(argv) < 2:
        print("Usage: visualize_cfg.py <xir_file>")
        exit(1)
    graphs = parse_control_flow_graphs_from_xir(argv[1])
    graph = graphviz.Digraph(format='pdf')
    for graph_desc in graphs:
        function = graph_desc['function'].replace("%", "func.")
        with graph.subgraph(name=f"cluster_{function}") as subgraph:
            subgraph.attr(label=f"Control Flow Graph of {function}")
            visualize_control_flow_graph(subgraph, graph_desc)
    graph.render(f"{argv[1]}.cfg")
