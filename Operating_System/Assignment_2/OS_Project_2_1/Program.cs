using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace OS_Project_2_1
{
    class Program
    {
        private static volatile string _str = "abc";
        private static volatile int _val = 0;
        static void AppendStr(string newstr)
        {
            while (true)
            {
                var original = Interlocked.CompareExchange(ref _str, null, null);
                var newString = original + newstr;
                if (Interlocked.CompareExchange(ref _str, newString, original) == original)
                    break;
            }
        }
        static void AddValue(int value)
        {
            while (true)
            {
                var orgVal = _val;
                var newVal = orgVal + value;
                if (Interlocked.CompareExchange(ref _val, newVal, orgVal) == orgVal)
                    break;
            }
        }
        static void ThreadBody()
        {
            for (int i = 0; i < 100000; i++)
            {  // Main 스레드와 병행 수행
                //_val++;       // #1-1) 단순한 덧셈 연산 _val++로 값 증가
                AddValue(1);    // #1-2) Atomic한 함수인 AddValue(1) 호출
            }
        }
        static void Main(string[] args)
        {
            Console.WriteLine("=====================================");
            //Console.WriteLine(" [세부과제 1-1] _val++ 연산");
            Console.WriteLine(" [세부과제 1-2] AddValue(1) 함수");
            Console.WriteLine(" - 컴퓨터과학부 2018920031 유승리");
            Console.WriteLine("=====================================");
            Console.WriteLine("Hello");
            AppendStr("xx");
            Console.WriteLine(_str);
            //            AddValue(5);
            //            Console.WriteLine(_val);
            Thread t1 = new Thread(ThreadBody);
            Thread t2 = new Thread(ThreadBody);
            Thread t3 = new Thread(ThreadBody);
            Thread t4 = new Thread(ThreadBody);
            Thread t5 = new Thread(ThreadBody);
            Thread t6 = new Thread(ThreadBody);
            Thread t7 = new Thread(ThreadBody);
            Thread t8 = new Thread(ThreadBody);
            Thread t9 = new Thread(ThreadBody);
            t1.Start();
            t2.Start();
            t3.Start();
            t4.Start();
            t5.Start();
            t6.Start();
            t7.Start();
            t8.Start();
            t9.Start();
            for (int i = 0; i < 100000; i++)
            {  // 스레드 t와 병행 수행
                //_val++;       // #1-1) 단순한 덧셈 연산 _val++로 값 증가
                AddValue(1);    // #1-2) Atomic한 함수인 AddValue(1) 호출
            }
            t1.Join();
            t2.Join();
            t3.Join();
            t4.Join();
            t5.Join();
            t6.Join();
            t7.Join();
            t8.Join();
            t9.Join();
            Console.WriteLine("-------------------------------------");
            //Console.WriteLine(" - Thread 개수: 5개 (Main 포함)");
            //Console.WriteLine(" - 예상 _val 출력: 500000");
            Console.WriteLine(" - Thread 개수: 10개 (Main 포함)");
            Console.WriteLine(" - 예상 _val 출력: 1000000");
            Console.WriteLine(" - 실제 _val 출력: " +_val);
            Console.WriteLine("=====================================");
        }
    }
}