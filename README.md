# Projeto-LI3

# Descrição
Este projeto foi feito no âmbito da unidade curricular Laboratórios de Informática III. Consiste em avaliar e validar o funcionamento e a eficiência do armazenamento e gestão da informação em memória dando ê resposta a um conjunto de interrogações
(queries) sobre os dados. A interação do utilizador com o programa é única e exclusivamente através da sua invocação via linha de comandos, seguindo os requisitos de cada modo de operação (batch ou interativo). 

## Funcionalidades
- **Parsing dos dados:** é realizada uma leitura dos ficheiros de entrada CSV e é efetuado um parsing genérico;
- **Interpretação dos comandos:** módulo responsável por ler o ficheiro de comandos, interpretar cada um e executar a respetiva query com os argumentos indicados;
- **Execução das interrogações(Queries):** módulo responsável por implementar a lógica das interrogações, delegando responsabilidades aos respetivos catálogos conforme necessário;
- **Output dos dados:** é realizada a escrita dos dados para a respetiva saída;
- **Catálogos de dados:** módulos responsáveis por armazenar e processar informações das diversas entidades, como utilizadores, voos, etc.;
- **Tipos e estruturas de dados:** Tipos e estruturas de dados necessários para a representação e armazenamento de dados, respetivamente;
- **Estatísticas:** módulo que efetua relações entre as várias entidades, proporcionando um acesso mais rápido a dados e resultados pedidos nas queries do programa;
- **Módulos de utilidade:** eventuais módulos extra necessários para o desenvolvimento da aplicação.
