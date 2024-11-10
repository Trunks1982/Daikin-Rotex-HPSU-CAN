import yaml

# add additional constructor
def secret_constructor(loader: yaml.SafeLoader, node: yaml.nodes.ScalarNode): 
    return "!secret " + f"{loader.construct_scalar(node)}"

yaml.SafeLoader.add_constructor('!secret', secret_constructor)

# YAML-Datei einlesen
with open('full.yaml', 'r') as file:
    config = yaml.safe_load(file)

# Gewünschten Block extrahieren
external_components_block = config.get('external_components')


print(yaml.safe_dump(external_components_block))
