all: cp

cp:
	gcc rt_verifica_conexao.c -o rt_verifica_conexao

run:
	sudo ./rt_verifica_conexao