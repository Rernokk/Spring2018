using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Grid_Manager : MonoBehaviour
{
  [SerializeField]
  GameObject quad;


  [Header("Player")]
  [SerializeField]
  GameObject player;

  [SerializeField]
  int playerX, playerY, playerZ;

  [SerializeField]
  GridSegment[] segmentArray;

  [HideInInspector]
  public GameObject[,,] gridCollection;
  [HideInInspector]
  public Node[,,] nodeCollection;

  [Serializable]
  struct GridSegment{
    public Vector3 gridDimensions;
    public Vector3 gridOrigin;
  }

  public int gridSizeX, gridSizeY, gridSizeZ;

  void Start()
  {
    GameObject gridHolder = new GameObject("Tile Parent");
    gridSizeX = 0;
    gridSizeY = 0;
    gridSizeZ = 0;
    foreach (GridSegment seg in segmentArray){
      gridSizeX = Math.Max((int)seg.gridDimensions.x + (int)seg.gridOrigin.x, gridSizeX);
      gridSizeY = Math.Max((int)seg.gridDimensions.y + (int)seg.gridOrigin.y, gridSizeY);
      gridSizeZ = Math.Max((int)seg.gridDimensions.z + (int)seg.gridOrigin.z, gridSizeZ);
    }
    gridCollection = new GameObject[gridSizeX, gridSizeY, gridSizeZ];
    nodeCollection = new Node[gridSizeX, gridSizeY, gridSizeZ];

    //TODO: Update for multiple grid segments
    foreach (GridSegment segment in segmentArray) {
      for (int i = 0; i < segment.gridDimensions.x; i++)
      {
        for (int j = 0; j < segment.gridDimensions.z; j++)
        {
          for (int k = 0; k < segment.gridDimensions.y; k++)
          {
            print(new Vector3(i, j, k) + new Vector3(segment.gridOrigin.x, segment.gridOrigin.y, segment.gridOrigin.z));
            gridCollection[i + (int)segment.gridOrigin.x, k + (int)segment.gridOrigin.z, j + (int)segment.gridOrigin.y] = Instantiate(quad, new Vector3(i, k, j) + new Vector3(segment.gridOrigin.x, segment.gridOrigin.y, segment.gridOrigin.z), Quaternion.Euler(new Vector3(90, 0, 0)));
            gridCollection[i + (int)segment.gridOrigin.x, k + (int)segment.gridOrigin.z, j + (int)segment.gridOrigin.y].transform.parent = gridHolder.transform;
            if (Vector3.Distance(new Vector3(i, 0, j), new Vector3(playerX, playerY, playerZ)) > 5.5)
            {
              gridCollection[i + (int)segment.gridOrigin.x, k + (int)segment.gridOrigin.z, j + (int)segment.gridOrigin.y].GetComponent<D_Tile_Color>().SetColor(new Color((float)(i) / gridSizeX, 0, (float)(j) / gridSizeZ));
            }
            else
            {
              gridCollection[i + (int)segment.gridOrigin.x, k + (int)segment.gridOrigin.z, j + (int)segment.gridOrigin.y].GetComponent<D_Tile_Color>().SetColor(new Color((float)(i) / gridSizeX, 1.2f - Vector3.Distance(new Vector3(i, k, j), new Vector3(playerX, playerY, playerZ)) / 5, (float)(j) / gridSizeZ));
            }
            nodeCollection[i + (int)segment.gridOrigin.x, k + (int)segment.gridOrigin.z, j + (int)segment.gridOrigin.y] = gridCollection[i + (int)segment.gridOrigin.x, k + (int)segment.gridOrigin.z, j + (int)segment.gridOrigin.y].transform.GetChild(0).GetComponent<Node>();
            nodeCollection[i + (int)segment.gridOrigin.x, k + (int)segment.gridOrigin.z, j + (int)segment.gridOrigin.y].GManager = this;
          }
        }
      }
    }

    StartCoroutine(StallNeighbors(1));
    player = Instantiate(player, new Vector3(playerX, playerY, playerZ), Quaternion.identity);
  }

  IEnumerator StallNeighbors(int frames)
  {
    for (int i = 0; i < frames; i++)
    {
      yield return null;
    }

    //for (int i = 0; i < gridSizeX; i++)
    //{
    //  for (int j = 0; j < gridSizeZ; j++)
    //  {
    //    for (int k = 0; k < gridSizeY; k++)
    //    {
    //      gridCollection[i, k, j].transform.GetChild(0).GetComponent<Node>().SetNeighbors();
    //    }
    //  }
    //}
    foreach (GameObject grid in gridCollection){
      if (grid != null && grid.transform.GetChild(0) != null)
      {
        grid.transform.GetChild(0).GetComponent<Node>().SetNeighbors();
      }
    }
  }

  public Node FetchNodeObject(int x, int z, int y)
  {
    if (gridCollection[x, z, y] != null && (x >= 0 && x < gridSizeX && z >= 0 && z < gridSizeZ && y >= 0 && y < gridSizeY))
    {
      print(gridCollection[x, y, z]);
      return gridCollection[x, y, z].transform.GetChild(0).GetComponent<Node>();
    }
    return null;
  }

  private void ResetGraph()
  {
    foreach (Node n in nodeCollection)
    {
      if (n != null)
      {
        n.Visited = false;
        n.BackNode = null;
      }
    }
  }

  private void DijkstrasReset()
  {
    foreach (Node n in nodeCollection)
    {
      if (n != null)
      {
        n.Visited = false;
        n.BackNode = null;
        n.CostSoFar = 1000000000;
      }
    }
  }

  public List<Transform> BreadthFirst(Vector3 start, Vector3 end)
  {
    List<Transform> path = new List<Transform>();
    ResetGraph();
    List<Node> ToVisit = new List<Node>();
    ToVisit.Add(nodeCollection[(int)start.x, (int)start.y, (int)start.z]);
    Node curr = ToVisit[0];
    Node startNode = curr;
    Node endNode = nodeCollection[(int)end.x, (int)end.y, (int)end.z];

    while (ToVisit.Count > 0)
    {
      curr = ToVisit[0];
      ToVisit[0] = null;
      ToVisit.RemoveAt(0);
      foreach (Node n in curr.Neighbors)
      {
        if (n != null && !n.Visited)
        {
          //Debug.DrawRay(curr.transform.position, n.transform.position - curr.transform.position, Color.red, 3f);
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
    ToVisit.Add(nodeCollection[(int)start.x, (int)start.y, (int)start.z]);
    Node curr = ToVisit[0];
    Node startNode = curr;
    Node endNode = nodeCollection[(int)end.x, (int)end.y, (int)end.z];
    startNode.CostSoFar = 0;
    while (ToVisit.Count > 0)
    {
      ToVisit.Sort((a, b) => a.CostSoFar.CompareTo(b.CostSoFar));
      curr = ToVisit[0];
      ToVisit[0] = null;
      ToVisit.RemoveAt(0);
      foreach (Node n in curr.Neighbors)
      {
        if (n != null)
        {
          float currDist = Vector3.Distance(n.transform.position, curr.transform.position);
          if (!n.Visited)
          {
            //Debug.DrawRay(curr.transform.position, n.transform.position - curr.transform.position, Color.red, 3f);
            n.BackNode = curr;
            n.Visited = true;
            n.CostSoFar = currDist + curr.CostSoFar;
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
          else
          {
            if (curr.CostSoFar + currDist <= n.CostSoFar)
            {
              n.CostSoFar = curr.CostSoFar + currDist;
              n.BackNode = curr;
            }
          }
        }
      }
    }
    return path;
  }

  public List<Transform> BestFirst(Vector3 start, Vector3 end)
  {
    List<Transform> path = new List<Transform>();
    DijkstrasReset();
    List<Node> ToVisit = new List<Node>();
    ToVisit.Add(nodeCollection[(int)start.x, (int)start.y, (int)start.z]);
    Node curr = ToVisit[0];
    Node startNode = curr;
    Node endNode = nodeCollection[(int)end.x, (int)end.y, (int)end.z];
    startNode.CostSoFar = 0;
    while (ToVisit.Count > 0)
    {
      ToVisit.Sort((a, b) => a.CostSoFar.CompareTo(b.CostSoFar));
      curr = ToVisit[0];
      ToVisit[0] = null;
      ToVisit.RemoveAt(0);
      foreach (Node n in curr.Neighbors)
      {
        if (n != null)
        {
          float currDist = Vector3.Distance(n.transform.position, endNode.transform.position);
          if (!n.Visited)
          {
            //Debug.DrawRay(curr.transform.position, n.transform.position - curr.transform.position, Color.red, 3f);
            n.BackNode = curr;
            n.Visited = true;
            n.CostSoFar = currDist + curr.CostSoFar;
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
          else
          {
            if (curr.CostSoFar + currDist <= n.CostSoFar)
            {
              n.CostSoFar = curr.CostSoFar + currDist;
              n.BackNode = curr;
            }
          }
        }
      }
    }
    return path;
  }
}