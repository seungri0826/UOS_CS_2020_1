using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Diagnostics;

namespace OS_Project_2_3
{
    public class Node
    {
        public Node next;
        public int data;

        public Node()
        {
            this.next = null;
            this.data = 0;
        }

        public Node(int data)
        {
            this.next = null;
            this.data = data;
        }
    }

    public class LinkedList
    {
        Node headNode; 
        static Mutex mtx = new Mutex();

        public LinkedList()
        {
            headNode = new Node();
        }

        public void InitFree()  // Free List에 노드 100개 넣어두는 메소드 (0~99)
        {
            for (int i = 0; i < 100; i++)  
            {
                Node tempNode = new Node(i);
                if (tempNode == null)
                {
                    Console.WriteLine("경고: 메모리가 부족합니다.");
                }
                tempNode.next = headNode.next;
                headNode.next = tempNode;
            }
        }

        public bool IsEmpty()   // LinkedList가 비어 있는지 여부를 반환하는 메소드
        {
            if (headNode.next == null)
                return true;
            return false;
        }

        public int GetLength()  // LinkedList의 노드 개수를 반환하는 메소드
        {
            int cnt = 0;
            Node countNode = headNode.next;
            while (countNode != null)
            {
                cnt++;
                countNode = countNode.next;
            }
            return cnt;
        }

        public void PrintNode() // LinkedList 내의 노드들의 데이터를 출력하는 메소드, 테스트용으로 사용
        {
            String content = "";
            Node tempNode = headNode.next;
            while (tempNode != null)
            {
                content = content + tempNode.data + ", ";
                tempNode = tempNode.next;
            }
            Console.WriteLine("Linked List 내용: " + content);
        }

        public void Push(Node newNode)  // Mutex를 이용한 Push 연산
        {
            if (newNode != null)
            {
                Node oldHead = new Node();
                mtx.WaitOne();
                oldHead.next = headNode.next;
                newNode.next = oldHead.next;
                headNode.next = newNode;
                mtx.ReleaseMutex();
            }
        }

        public Node Pop()   // Mutex를 이용한 Pop 연산
        {
            Node currentNode;
            mtx.WaitOne();
            currentNode = headNode.next;
            if(currentNode != null)
                headNode.next = currentNode.next;
            mtx.ReleaseMutex();
            return currentNode;
        }
    }

    class Program
    {
        private static volatile LinkedList freeList = new LinkedList(); // Free List
        private static volatile LinkedList headList = new LinkedList(); // Head List

        static void printStatus(String step)    // Free List와 Head List의 노드 개수를 출력하는 메소드
        {
            int freeLength = freeList.GetLength();
            int headLength = headList.GetLength();
            Console.WriteLine("-------------------------------------");
            Console.WriteLine(" [" + step + "]");
            Console.WriteLine(" - Free List 노드 개수: " + freeLength + "개");
            Console.WriteLine(" - Head List 노드 개수: " + headLength + "개");
            Console.WriteLine(" - 예상 전체 노드 개수: 100개");
            Console.WriteLine(" - 실제 전체 노드 개수: " + (freeLength + headLength) + "개");
        }

        static void ThreadBody()
        {
            int k = 0;
            Random r = new Random();
            while (k < 50000)
            {
                for (int i = 0; i < r.Next(0, 101); i++)
                {
                    if (!freeList.IsEmpty())
                        headList.Push(freeList.Pop());
                }
                for (int i = 0; i < r.Next(0, 101); i++)
                {
                    if (!headList.IsEmpty())
                        freeList.Push(headList.Pop());
                }
                Console.Write('\b');
                k++;
            }
        }

        static void Main(string[] args)
        {
            Console.WriteLine("=====================================");
            Console.WriteLine(" [세부과제 3]");
            Console.WriteLine(" - 컴퓨터과학부 2018920031 유승리");
            Console.WriteLine("=====================================");
            Console.WriteLine(" - Thread 개수: 5개");
            Console.WriteLine(" - 노드 개수: 100개");
            Console.WriteLine(" - 루프 반복 횟수: 50000회");

            freeList.InitFree();
            printStatus("초기");

            Thread t1 = new Thread(ThreadBody);
            Thread t2 = new Thread(ThreadBody);
            Thread t3 = new Thread(ThreadBody);
            Thread t4 = new Thread(ThreadBody);
            Thread t5 = new Thread(ThreadBody);
            Stopwatch stopwatch = new Stopwatch();

            stopwatch.Start();
            t1.Start();
            t2.Start();
            t3.Start();
            t4.Start();
            t5.Start();
            t1.Join();
            t2.Join();
            t3.Join();
            t4.Join();
            t5.Join();
            stopwatch.Stop();

            printStatus("완료");
            Console.WriteLine("-------------------------------------");
            Console.WriteLine(" - 걸린 시간: " + stopwatch.ElapsedMilliseconds + "ms");
            Console.WriteLine("=====================================");
        }
    }
}