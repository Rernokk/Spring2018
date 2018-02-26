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
}
