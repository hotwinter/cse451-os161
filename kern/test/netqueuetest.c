/*
 * netqueuetest.c -- Test implementation of fair network queue
 *
 */
#include <types.h>
#include <lib.h>
#include <thread.h>
#include <clock.h>
#include <synch.h>
#include <test.h>

#define NTHREADS 10
#define NPKTS 10

static int sender(void* p, unsigned long n);
static int receiver(void* p, unsigned long n);

/* a packet here is very simple -- it contains just the connection ID
 * and a sequence number */
struct packet {
  int connection;
  int sequenceNum;
};

/* network queue functions */
static void pktinit(void);
static void putpkt(int connection, struct packet *pkt);
static struct packet *getpkt(void);

static struct packet pkts[NTHREADS][NPKTS];

/* send 100 packets, each labelled with the thread ID */
int sender(void* p, unsigned long n)
{
  int i;

  (void)p;  // unused

  /* construct the pkts be sent */
  for(i = 0; i < NPKTS; i++) {
    pkts[n][i].connection = n;
    pkts[n][i].sequenceNum = i;
  }
  for(i = 0; i < NPKTS; i++) {
    putpkt(n, &pkts[n][i]);
    /* wait 100 usec to give a chance for other senders to run */
    clocksleep(1);
  }
  return 100 + n;
}

int receiver (void* p, unsigned long n)
{
  int i;
  struct packet *pkt;

  (void)p;  // unused

  for (i = 0; i < NTHREADS * NPKTS; i++) {
    pkt = getpkt();
    if (!pkt) {
      kprintf("Received NULL packet");
      thread_exit(-1);
    }
    kprintf("Got pkt: %d %d\n", pkt->connection, pkt->sequenceNum);
  }
  return n;
}


int netqueuetest(int argc, char ** argv)
{
  int i;
  int ret;
  int err;
  struct thread *senders[NTHREADS];
  struct thread *recvr;

  (void)argc;  // unused
  (void)argv;  // unused

  pktinit();  /* init the buffer */

  /* start the senders */
  for(i = 0; i < NTHREADS; i++) {
    thread_fork("send", &(senders[i]), NULL, &sender, NULL, i);
  }
  thread_fork("recv", &recvr, NULL, &receiver, NULL, NTHREADS);

  /* wait for everyone to finish */
  for(i = 0; i < NTHREADS; i++) {
    err = thread_join(senders[i], &ret);
  }
  err = thread_join(recvr, &ret);
  kprintf("Main thread done.\n");
  return ret;
}


/* The network queue implementation */

void pktinit()
{
  // Implement me!
}

void putpkt(int n, struct packet *pkt)
{
  (void)n;  // unused
  (void)pkt;  // unused
  // Implement me!
}

struct packet *getpkt()
{
  // Implement me!
  return NULL;
}
