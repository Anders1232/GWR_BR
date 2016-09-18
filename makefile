all:
	mkdir ../GWRrelease
	mkdir ../GWRrelease/Core/
	mkdir ../GWRrelease/DoubleMatrixLib/
	mkdir ../GWRrelease/LeitorArquivoSimples/
	mkdir ../GWRrelease/NamedColumnDoubleTable/
	mkdir ../GWRrelease/Qt/
	cp -dR Core/ ../GWRrelease
	cp -dR DoubleMatrixLib/ ../GWRrelease
	cp -dR LeitorArquivoSimples/ ../GWRrelease
	cp -dR NamedColumnDoubleTable/ ../GWRrelease
	cp -dR Qt/ ../GWRrelease
	cp -R GWR_BR.pro ../GWRrelease/GWR_BR.pro
	cd ../GWRrelease; \
	qmake GWR_BR.pro; \
	make; \
	cp GWR_BR ../GWR_BR/GWR_BR
	cd ../; \
	rm -R GWRrelease
gtk:
	make -f gtk.makefile

