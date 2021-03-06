package aterm.test;

import java.io.*;

import aterm.*;
import aterm.visitor.*;

public class VisitorBenchmark
{
  static int id = 0;
  static ATermFactory factory = new aterm.pure.PureFactory();
  static AFun fun;

  public final static void main(String[] args)
    throws IOException, VisitFailure
  {
    try {
      int depth  = Integer.parseInt(args[0]);
      int fanout = Integer.parseInt(args[1]);

      long beforeBuild = System.currentTimeMillis();
      fun = factory.makeAFun("f", fanout, false);
      ATerm term = buildTerm(depth, fanout);
      long beforeVisit = System.currentTimeMillis();
      NodeCounter nodeCounter = new NodeCounter();
      Visitor topDownNodeCounter = new TopDown(nodeCounter);
      try {
	topDownNodeCounter.visit(term);
	long end   = System.currentTimeMillis();
	System.out.println("Term of depth " + depth + " with fanout " + fanout
			   + " (" + nodeCounter.getCount() + " nodes)"
			   + ": build=" + (beforeVisit-beforeBuild)
			   + ", visit=" + (end-beforeVisit));
	//System.out.println("term = " + term);
      } catch (VisitFailure e) {
	System.err.println("WARING: VisitFailure: " + e.getMessage());
      }
    } catch (NumberFormatException e) {
      System.err.println("usage: java VisitorBenchmark <depth> <fanout>");
    }
  }

  private static ATerm buildTerm(int depth, int fanout)
  {
    if (depth == 1) {
      return factory.makeInt(id++);
    } else {
      ATerm[] args = new ATerm[fanout];
      ATerm arg = buildTerm(depth-1, fanout);
      for (int i=0; i<fanout; i++) {
	args[i] = arg;
      }
      return factory.makeAppl(fun, args);
    }
  }
}

class NodeCounter
  implements Visitor
{
  private int count;

  public void visit(Visitable visitable)
  {
    count++;
  }

  public int getCount()
  {
    return count;
  }
}

