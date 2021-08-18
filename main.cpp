/************************************************************
■副作用 (プログラム)
	https://ja.wikipedia.org/wiki/%E5%89%AF%E4%BD%9C%E7%94%A8_(%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%A0)
	
■C言語における副作用完了点とは？
	https://fromnand.hatenadiary.jp/entry/2020/04/01/212832

■副作用完了点について
	https://qiita.com/yz2cm/items/1b717ad13ea37c579793

■EXP30-C. 副作用が発生する式の評価順序に依存しない
	https://www.jpcert.or.jp/sc-rules/c-exp30-c.html#:~:text=%E5%BC%8F%E3%82%92%E8%A9%95%E4%BE%A1%E3%81%99%E3%82%8B%E3%81%93%E3%81%A8,(sequence%20point)%E3%81%A8%E5%91%BC%E3%81%B6%E3%80%82

■基礎から学ぶC言語講座
	https://www.cqpub.co.jp/toragi/TRBN/contents/2006/tr0610/0610soft5.pdf
	
■いろいろな式 | Programming Place Plus　Ｃ言語編　第２７章
	https://programming-place.net/ppp/contents/c/027.html
	
■rvalue reference 完全解説
	https://cpplover.blogspot.com/2009/11/rvalue-reference_23.html
************************************************************/
#include <stdio.h>
#include <iostream> 

#define TEST -1

/******************************
式		:	言語によって定められた優先順位や結びつきの規定によって評価される、
			値、変数、演算子、関数、の組み合わせ。
			式は、「評価された値」を持つ。
文		:	プログラム動作の最小構成単位。
			式分、if文、for文、while文、return 文、etcetc...
式分	:	単一の式から成る文。式の末尾に「;」を付けた文。

プログラミングにおいて、式の評価による作用には、主たる作用とそれ以外の副作用(side effect)とがある。
式は、評価値を得ることが主たる作用とされ、それ以外のコンピュータの論理的状態(ローカル環境以外の状態変数の値)を変化させる作用を副作用という。
副作用の例としては、グローバル変数や静的ローカル変数の変更、ファイルの読み書きなどのI/O実行、などがある。 

計算機言語における関数の副作用とは、値を求める以外の入出力や大域変数への代入を指します。言葉とは裏腹に、望んだはずの効果です。
	double func_x2(double& val){
		val *= 2;
		return val;
	}
	int main(){
		double val = 10;
		func_x2(val);
		
		return 0;
	}

式は他の式の一部になっていることがあります。
	b = a * 2
	f( a * 2 )
他の式の一部になっていない式は、完全式（完結式）と呼びます。

式を評価した結果、変数の値が変化するなど、実行環境に何らかの影響を与えることを、副作用といいます。
副作用は、特定のタイミングをもって確定します。このような位置を、副作用完了点といいます。逆にいえば、副作用完了点に到達するまで、副作用による変数の値の変化は確定していません。
代表的な副作用完了点は、以下のところにあります。
	-	完全式が終わった後
	-	すべての実引数を評価し終えた後、その関数内の処理へ移動する直前
			void func(int val_a, int val_b){
			}
			
			func(a++, b++);
			
	-	関数 返却値のコピー直後
	-	&&演算子や ||演算子 の１つ目のオペランドを評価した直後
			if( (a++ == 10) && (b++ == 10) ){
				...
			}
	-	カンマ演算子、条件演算子、の１つ目のオペランドを評価した直後
１つの変数の値を、副作用完了点に到達するまでの間に２回以上変更することは未定義の動作になるため、避けなければなりません。たとえば、「a = a++;」や「f( a++, a++ );」は問題があります。
以下はOK(関数 返却値のコピー直後に、一度、副作用完了点が入るので)。
	double x2(double& val){
		val *= 2;
		return val;
	}
	int main(){
		double a = 10;
		a = double(a);
	}
******************************/
#if (TEST == -1)
	void func(int val_a, int val_b){
		printf("%d, %d\n", val_a, val_b);
	}
	
	int main(){
		{
			int a = 0;
			int b = a++;
			printf("%d, %d\n", a, b);
		}
		
		{
			int a = 0;
			a = a++;		// 未定義
			printf("%d\n", a);
		}
		
		{
			int b = 0;
			func(b++, b++);	// 未定義
		}
		{
			int array[5];
			int i = 0;
			while (i < 5){
				// array[i++] = 0;	// ok.
				// array[i] = i++;	// 未定義
				array[i++] = i;	// 未定義
			}
		}
	}
	
/******************************
lvalue/ rvalue
	lvalue	:	明示的に実体のある、名前付きのオブジェクト
	rvalue	:	一時的に生成される無名のオブジェクト : 式の評価後、捨てられるオブジェクト。
******************************/
#elif (TEST == 0)
	struct X{} ;
	int f() { return 0 ; }
	
	int main()
	{
		int i = 0 ;
		
		i;		// lvalue
		0;		// rvalue
		
		X x;
		x;		// lvalue
		X();	// rvalue
		
		f();	// rvalue
	}
	/********************
	lvalueはrvalueに変換できるが、rvalueをlvalueに変換することは出来ない。
		f() = 10;
	とかは、不可、と言うこと。
	********************/
	
/******************************
reference
	C++98のreferenceは、C++11における、lvalue reference。
******************************/
#elif (TEST == 1)
	
	struct X{ };
	
	void f( X & ) { }
	void g( X const & ) { }
	
	int main()
	{
		X x ;
		f( x );			// 1. OK
		// f( X() );	// 2. Error : 注) VC++の独自拡張(/Zaで無効にできる)では、コンパイルが通ってしまうらしい。
		
		g( X() );		// 3. OK
		/********************
		3 : constなreferenceは、rvalueを参照できる。
		3.は、OKなのですが、言語的には、汚い仕様と言える。∵ rvalueなものを、lvalue referenceで参照しているため。
		->そこで、rvalue referenceの出番となる。
		********************/
	}
	
/******************************
rvalue reference
	rvalue referenceとは、その名の通り、rvalueに対する参照のこと(コードを見ればすぐ わかるはず)
******************************/
#elif (TEST == 2)
	struct X{ } ;
	
	int main()
	{
		X x ;
		
		// lvalue reference
		X& lr1 = x ;		// 1. OK
		X& lr2 = X() ;		// 2. Error
		
		// rvalue reference
		X&& rr1 = x ;		// 3. Error
		X&& rr2 = X() ;	// 4. OK
	}
	/********************
	rvalue referenceとは、
		「const lvalue referenceでなくてもrvalueを参照できるようになる」だけ。
		But :	そもそも、rvalueのオブジェクトには名前がなく、参照されなくなった時点で、自動的に破棄される。勝手に破棄されるなら、書き換えても無駄なはず。
				では、constではなくなったからといって、何がそんなに嬉しいのか。
	********************/
	
/******************************
Move Semantics
	シンタックス	＝ あるソースコードを“どう書く”か
	セマンティクス	＝ そのソースコードは“どう動く”か
******************************/
#elif (TEST == 3)
	#include <algorithm> // for std::copy
	
	class X
	{
	private :
		int* ptr = nullptr;
		
	public :
		X(){
			ptr = new int[10] ;
			for(int i = 0; i < 10; i++) { ptr[i] = i; }
		}
		
		X( X const& r ){
			ptr = new int[10] ;
			std::copy( &r.ptr[0], &r.ptr[10], &ptr[0] ) ;
		}
		
		~X(){
			/********************
			【C++ 小ネタ】delete演算子とfree関数はNULLチェックは不要
				https://flat-leon.hatenablog.com/entry/cpp_null_delete
			********************/
			if(ptr)	delete[] ptr ;
			ptr = nullptr;
		}
		
		void set(int id, int val){
			ptr[id] = val;
		}
		
		int get(int id){
			return ptr[id];
		}
	} ;
	
	int main()
	{
		X x_a;
		
		X x_b(x_a);
		for(int i = 0; i < 10; i++){
			printf("%d\n", x_b.get(i));
		}
	}
	/********************
	上の例では、明らかにコピーコンストラクタの実行が遅い。もし、コピーコンストラクタを、ポインタのすげ替え(コピーではない点に注意)だけにすれば、パフォーマンスが大いに向上するだろう。
	But :	そんなことをしてしまっては、コピー元のオブジェクトが使えなくなってしまうので、勝手にそんなことをしてはマズい。
			仮に、すげ替えでなく、shareにした場合は、memoryの管理(delete)を誰がするのか...
	
	ところが、ポインタをすげ替えても問題ないケースは確かに存在する。
	********************/

/******************************
Move コンストラクタ
******************************/
#elif (TEST == 4)
	#include <algorithm>
	
	class X{
	private :
		int* ptr = nullptr;
		
	public :
		X(){
			printf("default constructor\n");
			ptr = new int[10] ;
			for(int i = 0; i < 10; i++){
				ptr[i] = i;
			}
		}
		
		// copy constructor
		X( X const& r ){
			printf(">copy constructor : lvalue\n");
			ptr = new int[10] ;
			std::copy( &r.ptr[0], &r.ptr[10], &ptr[0] ) ;
		}
		
		// move constructor
		X( X&& r ){
			printf("move constructor\n");
			ptr = r.ptr ;
			r.ptr = nullptr; // r.ptrの所有権を放棄
		}
		
		~X(){
			release_mem();
		}
		
		void release_mem(){
			if(ptr) delete[] ptr ;
			ptr = nullptr;
		}
		
		void set(int id, int val){
			ptr[id] = val;
		}
		
		int get(int id){
			return ptr[id];
		}
		
		// 代入演算子
		X & operator = (X & r){
			printf(">operator=\n");
			
			if ( this == &r )
			return *this ;
			
			
			if(!ptr) ptr = new int[10] ;
			std::copy( &r.ptr[0], &r.ptr[10], &ptr[0] ) ;
			
			return *this ;
		}
		
		// move 代入演算子
		X & operator = (X && r){
			printf("move operator=(move)\n");
			
			if ( this == &r )
			return *this ;
			
			
			release_mem();
			
			ptr = r.ptr ;
			r.ptr = nullptr ; // r.ptrの所有権を放棄
			
			return *this ;
		}
	} ;
	
	/********************
	■[短文メモ][C++11]関数内のローカル変数を返すのに右辺値参照は要らない
		https://qiita.com/gyu-don/items/111634a4548b81b2d204
	********************/
	X f(){ return X() ; } // Return Value Optimization.
	
	int main()
	{
		/********************
		関数の戻り値はrvalueなので、
		Xのコンストラクタで、すげ変えらてていても、OK。
		********************/
		X a( f() ) ;
		for(int i = 0; i < 10; i++){
			printf("%d, ", a.get(i));
		}
		printf("\n");
		
		/********************
		ちなみに、コンパイラによっては、上記のコードは、そもそもコピーコンストラクタもmoveコンストラクタも呼び出されない可能性がある。
		というのも、ある種の状況においては、コンパイラは安全且つ静的に、オブジェクトをコピーせずに、使い回せることを決定できるのである。
		たとえコンストラクタにサイドエフェクトがあったとしても、コンストラクタの呼び出しを省略できるのである。この種の最適化は、規格で保証されている。
		********************/
		
		
		/********************
		「これ以降、tempはもう使わない」とprogrammerが勝手に決めた場合は ( tempは、lvalueなのだけど... )？
		********************/
		/*
		X tmp ; 
		// X b( tmp ) ; 
		// X b( static_cast<X &&>(tmp) ) ;
		X b( std::move(tmp) ) ; // std::move()とは、つまりキャスト動作のこと。
		*/
		
		/********************
		上で、copy constructor、move constructorは最適化されたが、下記の代入演算子は、最適化できないはず。
		また、実際のコードでは、movableにしたければ、move コンストラクタの他に、move 代入演算子も定義するべき。
		********************/
		/*
		X tmp ;
		X x ;
		
		x = std::move(tmp) ;
		*/
		
		/********************
		ひとたび、変数に対してstd::move()を呼び出すと、それ以降、その変数を使える保証はなくなる。なぜなら、すでにmoveされているかもしれないから。
		********************/
	}
	
/******************************
オーバーロード
******************************/
#elif (TEST == 5)
	
	struct X {} ;
	
	void f( X & x )
	{ std::cout << "lvalue reference" << std::endl ; }
	
	void f( X && x )
	{ std::cout << "rvalue reference" << std::endl ; }
	
	
	int main()
	{
		X x ;
		
		f( x ) ;	// lvalue reference
		f( X() ) ;	// rvalue reference
	}
	
/******************************
テンプレート関数の引数におけるrvalue referenceのargument deduction(テンプレート引数 推論)
******************************/
#elif (TEST == 6)
	
	struct X {} ;
	
	template < typename T >
	void f( T&& t ) {}
	
	int main()
	{
		X x ;
		
		f( x ) ;	// lvalue reference
		f( X() ) ;	// rvalue reference
	
	}
	/********************
	規格には特別なルールがあり、テンプレート引数を、rvalue referenceとして関数の引数に使った場合のargument deductionで、lvalueを渡すと、lvalue referenceとなるのである。
	つまり、上記のコードの場合、f()に、lvalue referenceを渡すと、TがX&になり、続く&&は無視され、lvalue referenceとして取り扱われる。
	実に不思議なルールである。しかし、これも理由あってのことなのだ。もし、これが出来ないとなると、
	プログラマは、わざわざ、lvalue referenceとrvalue referenceとで、似たようなコードを複数書かなければならなくなる。
	すべての組み合わせを網羅するには、膨大なオーバーロード関数が必要になる。
		引数が1個の場合は、オーバーロード関数は2個、
		引数が2個の場合は、4個、
		引数が3個の場合は、8個、
		引数が4個の場合は、16個
	...もの、オーバーロード関数を書かなければならない。これでは、一体何のためのテンプレートなのだろうか。
	幸いなことに、テンプレート関数の場合は、rvalue referenceでlvalue referenceも参照できるので、そのようなオーバーロード関数の指数関数的な増加は起こらない。
	
	しかし、ここでひとつ問題がある。
	********************/

/******************************
Perfect Forwarding
******************************/
#elif (TEST == 7)
	
	#include <algorithm>
	
	class X{
	private :
		int* ptr = nullptr;
		
	public :
		X(){
			printf(">default constructor\n");
			ptr = new int[10] ;
			for(int i = 0; i < 10; i++) { ptr[i] = i; }
		}
		
		X( X const& r ){
			printf(">copy constructor : lvalue\n");
			ptr = new int[10] ;
			std::copy( &r.ptr[0], &r.ptr[10], &ptr[0] ) ;
		}
		
		// move constructor
		X( X&& r ){
			printf(">move constructor : rvalue\n");
			ptr = r.ptr ;
			r.ptr = nullptr; // r.ptrの所有権を放棄
		}
		
		~X(){
			printf(">destructor\n");
			release_mem();
		}
		
		void release_mem(){
			printf("release_mem\n");
			if(ptr){
				printf("->delete\n");
				delete[] ptr ;
			}
			ptr = nullptr;
		}
		
		void set(int id, int val){
			ptr[id] = val;
		}
		
		int get(int id){
			return ptr[id];
		}
		
		// 代入演算子
		X & operator = (X & r){
			printf(">operator=\n");
			
			if ( this == &r )
			return *this ;
			
			
			if(!ptr) ptr = new int[10] ;
			std::copy( &r.ptr[0], &r.ptr[10], &ptr[0] ) ;
			
			return *this ;
		
		}
		
		// move 代入演算子
		X & operator = (X && r)
		{
			printf(">operator=(move)\n");
			
			if ( this == &r )
			return *this ;
			
			
			release_mem();
			
			ptr = r.ptr ;
			r.ptr = nullptr ;
			
			return *this ;
		}
	} ;
	
	/*
	template < typename T >
	void f( T && t )
	{
		if ( std::is_lvalue_reference<T>::value ) // メタプログラミング
			X x( t ) ;
		else
			X x( std::move(t) ) ;
	}
	*/
	/********************
	これは問題を多数のオーバーロード関数から、多数のメタプログラムに移しただけである。
	引数をひとつひとつ、このような方法で調べていくのは、面倒だし、引数が増えれば、オーバーロード関数と同じく、if文も爆発的に増えていく。
	********************/
	
	template < typename T >
	void f( T && t )
	{
		// X x( static_cast<T &&>(t) ) ; // argument deductionが働く
		X x( std::forward<T>(t) ) ;
	}
	
	/********************
	lvalue, rvalueの分岐を、各classに任せる。
	すると、必要なのは、lvalueの場合はlvalue、rvalueの場合はrvalueを渡す方法である。
	********************/
	
	int main()
	{
		X x ;
		f( x ) ;	// lvalue reference
		
		f( X() ) ;	// rvalue reference
	}
	

/******************************
最後に

rvalue referenceは、実に単純です。名前の通り、rvalueへの参照に過ぎません。

std::move()もstd::forward()は、単なるキャストに過ぎない.
std::move()は、lvalueをmoveしたいときに使い、std::forward()は、テンプレート関数の引数を、そのまま別の関数に渡したい時に使う。
******************************/



#endif

