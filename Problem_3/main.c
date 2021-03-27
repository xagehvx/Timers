void printt(){

	int aa = 999;

	int bb = aa/10;
	int basamak0 = aa-(bb*10);

	bb = aa/100;
	int basamak1 = (aa-(bb*100))/10;

	bb = aa/1000;
	int basamak2 = (aa-(bb*1000))/100;

	bb = aa/10000;
	int basamak3 = (aa-(bb*10000))/1000;


	for(int i = 0; i < 100; i++ ){

		print_b(basamak0, 0);
		print_b(basamak1, 1);
		print_b(basamak2, 2);
		print_b(basamak3, 3);

	}

	aa--;
}

	print_b(int x, int y){
		switch(y){

		case 0:
			0b10000
			print_c(x);
			break;

		case 1:
			0b100000
			print_c(x);
			break;

		case 2:
			0b1000000
			print_c(x);
			break;

		case 3:
			0b10000000
			print_c(x);
			break;
		}

	}

	print_c(int x){
		switch (x)
		{
		case 0:
			0b1000000
			break;

		case 1:
			0b1111001
			break;

		case 2:
			0b0100100
			break;

		case 3:
			0b0110000
			break;

		case 4:
			0b0011001
			break;

		case 5:
			0b0010010
			break;

		case 6:
			0b0000010
			break;

		case 7:
			0b1111000
			break;

		case 8:
			0b0
			break;

		case 9:
			0b0010000
			break;

		default:
			break;
		}
	}

int main(){

	return 0;
}
