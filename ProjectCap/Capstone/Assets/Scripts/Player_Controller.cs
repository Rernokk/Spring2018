using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player_Controller : MonoBehaviour {
  [SerializeField]
  Vector3 targetPosition = Vector3.zero;

  Grid_Manager gridManager;
  List<Transform> path;

	// Use this for initialization
	void Start () {
    gridManager = GameObject.FindGameObjectWithTag("Grid_Manager").GetComponent<Grid_Manager>();
    path = new List<Transform>();
	}
	
	// Update is called once per frame
	void Update () {
    if (Input.GetKeyDown(KeyCode.B))
    {
      path = gridManager.BreadthFirst(transform.position, targetPosition);
      for (int i = 0; i < path.Count - 1; i++)
      {
        Debug.DrawRay(Vector3.up * .5f + path[i].position, path[i + 1].position - path[i].position, Color.yellow, 3f);
      }
    }
    if (Input.GetKeyDown(KeyCode.D)){
      path = gridManager.Dijkstras(transform.position, targetPosition);
      for (int i = 0; i < path.Count - 1; i++){
        Debug.DrawRay(Vector3.up * .5f + path[i].position, path[i+1].position - path[i].position, Color.yellow, 3f);
      }
    }
    if (Input.GetKeyDown(KeyCode.S))
    {
      path = gridManager.BestFirst(transform.position, targetPosition);
      for (int i = 0; i < path.Count - 1; i++)
      {
        Debug.DrawRay(Vector3.up * .5f + path[i].position, path[i + 1].position - path[i].position, Color.yellow, 3f);
      }
    }
  }
}
