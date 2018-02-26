using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Grid_Manager : MonoBehaviour
{
  [SerializeField]
  GameObject quad;

  [SerializeField]
  int gridSizeX, gridSizeZ;

  [Header("Player")]
  [SerializeField]
  GameObject player;

  [SerializeField]
  int playerX, playerZ;

  [HideInInspector]
  public GameObject[,] gridCollection;
  [HideInInspector]
  public Node[,] nodeCollection;

  void Start()
  {
    GameObject gridHolder = new GameObject("Tile Parent");
    gridCollection = new GameObject[gridSizeX, gridSizeZ];
    nodeCollection = new Node[gridSizeX, gridSizeZ];
    for (int i = 0; i < gridSizeX; i++)
    {
      for (int j = 0; j < gridSizeZ; j++)
      {
        gridCollection[i, j] = Instantiate(quad, new Vector3(i, 0, j), Quaternion.Euler(new Vector3(90, 0, 0)));
        gridCollection[i, j].transform.parent = gridHolder.transform;
        if (Vector3.Distance(new Vector3(i, 0, j), new Vector3(playerX, 0, playerZ)) > 5.5)
        {
          gridCollection[i, j].GetComponent<D_Tile_Color>().SetColor(new Color((float)(i) / gridSizeX, 0, (float)(j) / gridSizeZ));
        }
        else
        {
          gridCollection[i, j].GetComponent<D_Tile_Color>().SetColor(new Color((float)(i) / gridSizeX, 1.2f - Vector3.Distance(new Vector3(i, 0, j), new Vector3(playerX, 0, playerZ)) / 5, (float)(j) / gridSizeZ));
        }
        nodeCollection[i, j] = gridCollection[i, j].transform.GetChild(0).GetComponent<Node>();
        nodeCollection[i, j].GManager = this;
      }
    }

    StartCoroutine(StallNeighbors(1));
    player = Instantiate(player, new Vector3(playerX, 0, playerZ), Quaternion.identity);
  }

  IEnumerator StallNeighbors(int frames)
  {
    for (int i = 0; i < frames; i++)
    {
      yield return null;
    }

    for (int i = 0; i < gridSizeX; i++)
    {
      for (int j = 0; j < gridSizeZ; j++)
      {
        gridCollection[i, j].transform.GetChild(0).GetComponent<Node>().SetNeighbors();
      }
    }
  }

  public Node FetchNodeObject(int x, int z)
  {
    if (x >= 0 && x < gridSizeX && z >= 0 && z < gridSizeZ)
    {
      return gridCollection[x, z].transform.GetChild(0).GetComponent<Node>();
    }
    return null;
  }

  private void ResetGraph()
  {
    foreach (Node n in nodeCollection)
    {
      n.Visited = false;
      n.BackNode = null;
    }
  }

  private void DijkstrasReset(){
    foreach (Node n in nodeCollection){
      n.Visited = false;
      n.BackNode = null;
      n.CostSoFar = 1000000000;
    }
  }

  public List<Transform> BreadthFirst(Vector3 start, Vector3 end)
  {
    List<Transform> path = new List<Transform>();
    ResetGraph();
    List<Node> ToVisit = new List<Node>();
    ToVisit.Add(nodeCollection[(int)start.x, (int)start.z]);
    Node curr = ToVisit[0];
    Node startNode = curr;
    Node endNode = nodeCollection[(int)end.x, (int)end.z];

    while (ToVisit.Count > 0)
    {
      curr = ToVisit[0];
      ToVisit[0] = null;
      ToVisit.RemoveAt(0);
      foreach (Node n in curr.Neighbors)
      {
        if (n != null && !n.Visited)
        {
          Debug.DrawRay(curr.transform.position, n.transform.position - curr.transform.position, Color.red, 3f);
          n.BackNode = curr;
          n.Visited = true;
          ToVisit.Add(n);
          if (n == endNode)
          {
            print("Found End");
            curr = n;
            startNode.BackNode = null;
            while (curr != null)
            {
              path.Add(curr.transform);
              curr = curr.BackNode;
            }
            return path;
          }
        }
      }
    }
    return path;
  }

  public List<Transform> Dijkstras(Vector3 start, Vector3 end)
  {
    List<Transform> path = new List<Transform>();
    DijkstrasReset();
    List<Node> ToVisit = new List<Node>();
    ToVisit.Add(nodeCollection[(int)start.x, (int)start.z]);
    Node curr = ToVisit[0];
    Node startNode = curr;
    Node endNode = nodeCollection[(int)end.x, (int)end.z];
    startNode.CostSoFar = 0;

    while (ToVisit.Count > 0)
    {
      curr = ToVisit[0];
      ToVisit[0] = null;
      ToVisit.RemoveAt(0);
      foreach (Node n in curr.Neighbors)
      {
        float currDist = Vector3.Distance(n.transform.position, curr.transform.position);
        if (n != null && !n.Visited)
        {
          Debug.DrawRay(curr.transform.position, n.transform.position - curr.transform.position, Color.red, 3f);
          n.BackNode = curr;
          n.Visited = true;
          ToVisit.Add(n);
          if (n == endNode)
          {
            print("Found End");
            curr = n;
            startNode.BackNode = null;
            while (curr != null)
            {
              path.Add(curr.transform);
              curr = curr.BackNode;
            }
            return path;
          }
        } else {
          if (curr.CostSoFar + currDist <= n.CostSoFar){
            n.CostSoFar = curr.CostSoFar + currDist;
            n.BackNode = curr;
          }
        }
      }
    }
    return path;
  }
}
//Python Dijkstra's
/*
def DijkstrasSearch(nodeList, startNode, endNode):
  toVisit = [startNode]
	path = []
	for nodeRow in nodeList:
		for node in nodeRow:
			if (node != 0):
				node.visited = False
				node.backNode = 0
				node.costSoFar = 3000000
				node.col = (255,255,255)
			
	startNode.costSoFar = 0
	curr = startNode
	curr.backNode = 0

	while (len(toVisit) > 0):
		curr = toVisit.pop(0)
		if (curr != 0):
			for neighbor in curr.neighbors:
				if (neighbor[0] != 0):
					currDist = neighbor[1]#curr.position.distance(neighbor[0].position)
					if(neighbor[0].visited == False):
						#if (neighbor[0] not in toVisit):
						toVisit.append(neighbor[0])
						neighbor[0].visited = True
						neighbor[0].backNode = curr
						neighbor[0].costSoFar = currDist + curr.costSoFar
						if (neighbor[0] == endNode):
							total = neighbor[0].costSoFar
							startNode.backNode = 0
							path.append(neighbor[0])
							while (curr != 0):
								path.append(curr)
								curr = curr.backNode
							startNode.col = (255,0,0)
							endNode.col = (0,0,0)
							return path, total
					else:
						#print("Found Visited, Current: ",curr.costSoFar + currDist, ", neighbor: ", neighbor[0].costSoFar)
						if (curr.costSoFar + currDist <= neighbor[0].costSoFar):
							neighbor[0].costSoFar = curr.costSoFar + currDist
							neighbor[0].backNode = curr
	return path, 0
*/