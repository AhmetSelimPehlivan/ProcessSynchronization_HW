# ProcessSynchronization_HW
Compile etmek için
$ gcc -Wall -o "hw2" "hw2.c" 
yapabilirsiniz.
$ ./hw2 
Şeklinde Execute edebilirsiniz.

>Kodda variable'ları dynamic olarak malloc ile allocate ettim ve threadler ölmeden önce memory'yi freeledim ki 
	memory leak hatası almayayım.
>Waiter Philosopher arasındaki synchronization problemini product-consumer problemi mantığı ile çözmeye çalıştım.
>Pilav bittiğinde philosophların durması için yeni bir pthread_mutex_lock tanımladım.
>Count arttırırken bir sıkıntı çıkmaması için pthread_mutex_lock ve pthread_mutex_unlock içine aldım.
>Kodun temelini geekforgeeksden aldım. https://www.geeksforgeeks.org/dining-philosopher-problem-using-semaphores/

Ahmet Selim Pehlivan