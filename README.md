# CVRP-heteregeneos
<p align="center">Problema de Roteamento de Veículos com Capacidade Heterogênea em C++</p>
<h4 align="center">
  ⚠️ Projeto em construção ⚠️
</h4>

### Features
- [X] Implementar o KMeans com silloute modificado
- [X] Dividir o mapa em **R** regiões, com cada região com **N'** pacotes.
- [X] Para cada região **R** selecionar os **K** veículos que estarão destinados à região.
- [X] Selecionar quais pacotes **N'** entraram no veículo **V**
- [X] Criação dos modelos matemáticos.
  - [X] Modelo para otimizar quais veículos irão para determinada região.
  - [X] Modelo para otimizar quais pacotes deverão entrar em determinado veículo. 
  - [X] Modelo para resolver o CVRP
- [ ] Integração do desenvolvimento do VRP com Relax-and-Fix 

# Sobre

Para poder resolver o CVRP heterogeneo, pode ser visto de diversas maneiras. Uma delas é que primeiramente necessitamos escolher quais veículos poderão ser utilizados. Resolver o problema em um único modelo matemático, pode se tornar mais custoso do que separar o único problema que temos em outros modelos matemáticos que possuem etapas diferentes para serem resolvidas. Neste caso no **Modelo I** é separado a dispersão dos pacotes em **R** regiões, que são calculadas através do KMeans. O primeiro passo é dividir os **N** pacotes para **R** regiões, nas quais cada uma será atendida por **K'** veículos. Neste passo eliminamos a possibilidade de um veículo atender mais de uma região, entretanto estaremos ganhando um pouco mais de tempo para não resolver veículos em mais de uma região. O segundo passo é utilizar o **Modelo II** que tem como objetivo distribuir os **N** pacotes de cada região para os veículos presentes desta região. Por fim o **Modelo III** têm o objetivo de resolver os caminhos de cada veículo. 

# Modelos Matemáticos
# Modelo I
# Modelo II
# Modelo III


# Contato

- https://www.linkedin.com/in/wilton-costa-27a3b2143/
