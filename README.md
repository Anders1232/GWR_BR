# GWR BR
Projeto de iniciação científica(PIBIC)

Desenvolvimento de Aplicativo em Linguagem C para Análise de Modelos de Regressão Geograficamente Ponderados

Núcleo de processamento do programa feito em C, interface gráfica feita em C++ usando Qt, porque o gtk+ 3 não funciona no Windows.

The main.cpp and GuiInterface.hpp should main.c and GuiInterface.h; but if I do this the Qt can't compile

Descrição das pastas:
	Core: Contém código C com funcionalidades cetrais
	DoubleMatrixLib: Biblioteca de matrizes de elementos do tipo double
	GWR-Lib: provê para as DoubleMatrix funcionalidades/operações relativas à Regressões Geograficamente Ponderadas
	img: contém imagens utilizadas
	NamedColumnDoubleTable: Contém uma extensão do DoubleMatrix capaz de nomear as colunas
	Qt: Arquivos específicos da interface gráfica Qt
	SampleData: dados de teste
