struct Factura
{
	int id;
	int cliente;
	int producto[100];
	int cantidad[100];
	int total;
	int items;
	bool estado;
}factura, *pFactura = &factura;

bool crear_factura()
{
	FILE *archivo;
	Producto pro, *pPro = &pro;
	int cantidad = 0, product = 1, cantProducto = 0, y = 0, i=1, total = 0;
	int* pCantidad = &cantidad, *pProduct = &product, *pCantProducto = &cantProducto, *pTotal = &total;
	char pagada;
	bool bandera = false;
	
	archivo = fopen("datos/factura.txt", "rb");
	
	if(archivo == NULL)
	{
		return false;
	}
	rewind(archivo);
	fseek(archivo,ftell(archivo)-sizeof(*pFactura),SEEK_END);
	fread(pFactura, sizeof(*pFactura), 1, archivo);
	gotoxy(31, 2);printf("Seleccione Cliente");
	listar_cliente(3);
	pFactura->id = pFactura->id+1;
	fclose(archivo);
	color(amarillo);
	fflush(stdin);
	y = wherey();
	while(!bandera)
	{
		limpiaPantalla(29,y+2, 50, wherey()+4);
		color(amarillo);
		gotoxy(31, y+2);printf("Id del cliente: ");
		color(grisclaro);
		scanf("%d", &pFactura->cliente);
		if(buscar_cliente(&pFactura->cliente) == 0)
		{
			color(rojo);
			gotoxy(29, y+4);printf("El cliente no existe");
			getch();
		}
		else
		{
			limpiaPantalla(29,y+4, 50, wherey()+4);
			bandera = true;
		}
	}
	gotoxy(30, y+4);printf("Seleccione Productos");
	listar_productos(y+6);
	y = wherey();
	color(amarillo);
	fflush(stdin);
	gotoxy(18, y+2);printf("Ingrese los id de productos (0 - finaliza)");
	while(*pProduct != 0)
	{
		bandera = false;
		while(!bandera)
		{
			color(amarillo);
			gotoxy(32, y+i+3);printf("Producto %d: ", *pCantidad+1);
			color(grisclaro);
			scanf("%d", pProduct);
			if(*pProduct > 0)
			{
				if(buscar_producto(pProduct) == 0)
				{
					color(rojo);
					gotoxy(30, y+i+4);printf("Producto no existe");
					getch();
					limpiaPantalla(30,y+i+3,50,y+i+6);
				}
				else
				{
					i++;
					pFactura->producto[*pCantidad] = *pProduct;
					while(!bandera)
					{
						gotoxy(28, y+i+4);printf("Cantidad Producto %d: ", *pCantidad+1);
						scanf("%d", pCantProducto);
						
						datos_productos(pProduct, pPro);
						if(*pCantProducto > pPro->cantidad)
						{
							color(rojo);
							gotoxy(30, y+i+5);printf("Cantidad excedida");
							getch();
							printf("%d -- \n",y);
							limpiaPantalla(30,y+i+4,50,y+i+7);
						}
						else
						{
							pFactura->cantidad[*pCantidad] = *pCantProducto;
							
							*pTotal = *pTotal + (pPro->valor * pFactura->cantidad[*pCantidad]);
							(*pCantidad)++;
							pFactura->items = *pCantidad; 
							i+=3;
							bandera = true;
						}
					}
				}
			}
			else
			{
				break;
			}
		}
	}
	pFactura->total = *pTotal;
	fflush(stdin);
	gotoxy(28, wherey()+2);printf("Factura pagada S / N: ");
	scanf("%c", &pagada);
	if(pagada == 's'|| pagada == 'S')
	{
		pFactura->estado = true;
	}
	else
	{
		pFactura->estado = false;
	}
	archivo = fopen("datos/factura.txt", "ab");
	
	if(archivo == NULL)
	{
		color(rojoclaro);
		gotoxy(24, wherey()+2);printf("No se pudo guardar el registro");
	}
	else
	{
		fwrite(pFactura,sizeof(*pFactura),1,archivo);
		rewind(archivo);
		fclose(archivo);
		color(verdeclaro);
		gotoxy(31, wherey()+2);printf("Registro Exitos");
	}
}


bool listar_facturas()
{
	FILE *archivo;
	int y = 7, id = 0, *pId = &id, i;
	Cliente cli, *pCli = &cli;
	
	archivo = fopen("datos/factura.txt", "rb");
	
	if(archivo == NULL)
	{
		return false;
	}
	else
	{
		system("mode con: cols=60 lines=50");
		gotoxy(25, 2);printf("Listado de Facturas");
		i = 4;
		while(fread(pFactura, sizeof(*pFactura), 1, archivo))
		{
			*pId = pFactura->cliente;
			datos_cliente(pId, pCli);
			
			color(amarillo);
			dibujaRectangulo(2,i-1,52,i+1,'\372');
			color(grisclaro);
			gotoxy(3, i);printf("%d", pFactura->id);
			color(amarillo);
			gotoxy(7, i);printf("\263");
			color(grisclaro);
			gotoxy(8, i);printf("%s", pCli->nombre);
			color(amarillo);
			gotoxy(38, i);printf("\263");
			color(grisclaro);
			gotoxy(39, i);printf("%d", pFactura->items);
			color(amarillo);
			gotoxy(43, i);printf("\263");
			color(grisclaro);
			gotoxy(44, i);printf("%d", pFactura->total);
			i+=2;
		}
		fclose(archivo);
	}
}

void mostrar_factura(int *pId)
{
	FILE *archivo;
	Cliente cli, *pCli = &cli;
	Producto pro, *pPro = &pro;
	int id = 0, idPro = 0;
	int *pIdc = &id, *pIdPro = &idPro;
	archivo = fopen("datos/factura.txt", "rb");
	
	while(fread(pFactura, sizeof(*pFactura), 1, archivo))
	{
		if(*pId == pFactura->id)
		{
			*pIdc = pFactura->cliente;
			datos_cliente(pIdc, pCli);
			system("mode con: cols=90 lines=50");
			color(verdeclaro);
			gotoxy(41,2);printf("Factura");
			gotoxy(3,5);printf("Id - factura: ");
			printf("%d", pFactura->id);
			color(amarillo);
			dibujaRectangulo(2,9,80,11,'\372');
			color(grisclaro);
			gotoxy(3,10);printf("Cedula");
			color(amarillo);
			gotoxy(14, 10);printf("\263");
			color(grisclaro);
			gotoxy(15,10);printf("Nombre");
			color(amarillo);
			gotoxy(36, 10);printf("\263");
			color(grisclaro);
			gotoxy(37,10);printf("Direccion");
			color(amarillo);
			gotoxy(68, 10);printf("\263");
			color(grisclaro);
			gotoxy(69,10);printf("Telefono");
			color(amarillo);
			dibujaRectangulo(2,11,80,13,'\372');
			gotoxy(3,12);printf("%s", pCli->cedula);
			color(amarillo);
			gotoxy(14, 12);printf("\263");
			color(grisclaro);
			gotoxy(15,12);printf("%s", pCli->nombre);
			color(amarillo);
			gotoxy(36, 12);printf("\263");
			color(grisclaro);
			gotoxy(37,12);printf("%s", pCli->direccion);
			color(amarillo);
			gotoxy(68, 12);printf("\263");
			color(grisclaro);
			gotoxy(69,12);printf("%s", pCli->telefono);
			
			gotoxy(40,15);printf("Productos");
			
			color(amarillo);
			dibujaRectangulo(2,17,80,19,'\372');
			gotoxy(3,18);printf("item");
			color(amarillo);
			gotoxy(7, 18);printf("\263");
			color(grisclaro);
			gotoxy(8,18);printf("Nombre");
			color(amarillo);
			gotoxy(39, 18);printf("\263");
			color(grisclaro);
			gotoxy(40,18);printf("Cantidad");
			color(amarillo);
			gotoxy(51, 18);printf("\263");
			color(grisclaro);
			gotoxy(52,18);printf("Valor");
			color(amarillo);
			gotoxy(63, 18);printf("\263");
			color(grisclaro);
			gotoxy(64,18);printf("Subtotal");
			int k = 0;
			int j = 2;
			for(int i = 0; i < pFactura->items; i++)
			{
				*pIdPro = pFactura->producto[i];
				datos_productos(pIdPro, pPro);
				
				color(amarillo);
				dibujaRectangulo(2,17+j,80,19+j,'\372');
				gotoxy(3,18+j);printf("%d", k+1);
				color(amarillo);
				gotoxy(7, 18+j);printf("\263");
				color(grisclaro);
				gotoxy(8,18+j);printf("%s", pPro->nombre);
				color(amarillo);
				gotoxy(39, 18+j);printf("\263");
				color(grisclaro);
				gotoxy(40,18+j);printf("%d", pFactura->cantidad[i]);
				color(amarillo);
				gotoxy(51, 18+j);printf("\263");
				color(grisclaro);
				gotoxy(52,18+j);printf("%.2f", pPro->valor);
				color(amarillo);
				gotoxy(63, 18+j);printf("\263");
				color(grisclaro);
				gotoxy(64,18+j);printf("%.2f", (pPro->valor*pFactura->cantidad[i]));
				j+=2;
				k++;
				
			}
			color(amarillo);
			dibujaRectangulo(2,17+j,80,19+j,'\372');
			gotoxy(52,18+j);printf("Total:");
			gotoxy(64,18+j);printf("%d", pFactura->total);
			
		}
	}
	fclose(archivo);
}

void editar_factura(int *pId){
	
	int opcion = 0, *pOpcion = &opcion, id;
	Producto pro, *pPro = &pro;
	FILE *archivo;
	archivo = fopen("datos/factura.txt", "rb");
	fseek(archivo,sizeof(*pFactura)* ((*pId) - 1),SEEK_SET);
	fread(pFactura, sizeof(*pFactura), 1, archivo);
	
	do{
		color(amarillo);
		int y = wherey()+2;
		gotoxy(30,y);printf(" EDITAR FACTURA ");
		gotoxy(17,y+1);printf("1. Cambiar Cliente ");
		gotoxy(17,y+2);printf("2. Cambiar Productos ");
		gotoxy(17,y+3);printf("0. Salir");
		color(verde);
		gotoxy(17,y+6);printf(" Seleccione la opcion que desea Modificar: ");
		scanf("%d", &opcion);
		switch (*pOpcion)
		{
			case 1:
			{
				listar_cliente(wherey()+2);
				gotoxy(28,y+8);printf("Ingrese el nuevo id para el cliente: ");
				scanf("%d", &pFactura->cliente);
				fclose(archivo);
				archivo = fopen("datos/factura.txt", "rb+");
				fseek(archivo,sizeof(*pFactura)* ((*pId) - 1),SEEK_SET);
				fwrite(pFactura,sizeof(*pFactura),1,archivo);
				fclose(archivo);
				system("cls");
				mostrar_factura(pId);
				editar_factura(pId);
				break;
			}
			case 2:
			{
				gotoxy(28,y+8);printf("Id de producto que desea cambiar: ");
				scanf("%d", &id);
				listar_productos(y+10);
				y = wherey();
				gotoxy(28,y+2);printf("Ingrese el nuevo id para el producto; ");
				scanf("%d", &pFactura->producto[id-1]);
				gotoxy(28,y+4);printf("Ingrese la nueva cantidad del producto; ");
				scanf("%d", &pFactura->cantidad[id-1]);
				pFactura->total = 0;
				
				for(int i = 0; i<pFactura->items;i++)
				{
					datos_productos(&pFactura->producto[i], pPro);
					pFactura->total = pFactura->total+(pFactura->cantidad[i] * pPro->valor);
				}
				fclose(archivo);
				archivo = fopen("datos/factura.txt", "rb+");
				fseek(archivo,sizeof(*pFactura)* ((*pId) - 1),SEEK_SET);
				fwrite(pFactura,sizeof(*pFactura),1,archivo);
				fclose(archivo);
				system("cls");
				mostrar_factura(pId);
				editar_factura(pId);
				break;
			}
			
		}
		if(*pOpcion != 0)
		{
			system("cls");
			//producto_encontrado(pUbicacion);
		}
	}while(*pOpcion!=0);
	fclose(archivo);
}
	   


