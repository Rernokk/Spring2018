using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Node : MonoBehaviour
{
  Grid_Manager manager;
  Node[,] neighbors;
  Node backNode;
  bool visited;
  float costSoFar;
  public Grid_Manager GManager
  {
    get
    {
      return manager;
    }

    set
    {
      manager = value;
    }
  }
  public Node BackNode
  {
    get
    {
      return backNode;
    }

    set
    {
      backNode = value;
    }
  }
  public bool Visited
  {
    get
    {
      return visited;
    }

    set
    {
      visited = value;
    }
  }
  public Node[,] Neighbors
  {
    get
    {
      return neighbors;
    }
  }
  public float CostSoFar
  {
    get
    {
      return costSoFar;
    }

    set
    {
      costSoFar = value;
    }
  }
  void Awake()
  {
    neighbors = new Node[3, 3];
    GManager = null;
    BackNode = null;
    Visited = false;
  }


  public void SetNeighbors()
  {
    for (int i = -1; i <= 1; i++)
    {
      for (int j = -1; j <= 1; j++)
      {
        if (i != 0 || j != 0)
        {
          Node n = GManager.FetchNodeObject(i + (int)(transform.position.x), j + (int)(transform.position.z));
          neighbors[i + 1, j + 1] = n;
        }
      }
    }
  }
}