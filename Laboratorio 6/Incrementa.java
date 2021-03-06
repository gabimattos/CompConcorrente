//classe que implementa a tarefa das threads do programa

class Thr extends Thread {
	//id da thread
	private int id;
	
	//objetos compartilhados pelas threads
	public int[] vetor;
	public int nthreads;

	//construtor
	public Thr(int tid, int[] vetor, int nthreads) {
		this.id = tid;
		this.vetor = vetor;
		this.nthreads = nthreads;
	}

	//main da thread
	public void run() {
		for (int i=this.id; i<this.vetor.length; i+=nthreads) {
			this.vetor[i]++;
		}
	}
}

//main da aplicação
class Principal {
	public static void main (String[] args) {
		if(args.length < 2){
			System.out.println("java Principal <num de threads> <tam do vetor>");
			System.exit(1);
		}
		
		//tam do vetor
		int tam = Integer.parseInt(args[1]);
		//num de threads
		int nthreads = Integer.parseInt(args[0]) > tam ? tam : Integer.parseInt(args[0]);
		//reserva espaço para um vetor de threads
		Thread[] threads = new Thread[nthreads];

		//cria o vetor que é compartilhado entre as threads
		int[] vetor = new int[tam];
		for (int i = 0; i< tam; i++ ) {
			vetor[i] = 0;
			System.out.printf("%d ",vetor[i]);
		}
		System.out.printf("\n");
		
		//cria as threads e as inicializa
		for (int i=0; i<threads.length; i++) {
		   threads[i] = new Thr(i, vetor, nthreads);
		}

		for (int i=0; i<threads.length; i++) {
		   threads[i].start();
		}

		//espera pelo termino das threads
		for (int i=0; i<threads.length; i++) {
		   try { threads[i].join(); } catch (InterruptedException e) { return; }
		}
		for (int i = 0; i< tam; i++) {
			System.out.printf("%d ",vetor[i]);
		}
		System.out.printf("\n");
	}
}
