using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player_Controller : MonoBehaviour {
  Grid_Manager gridManager;
  List<Transform> path;
	// Use this for initialization
	void Start () {
    gridManager = GameObject.FindGameObjectWithTag("Grid_Manager").GetComponent<Grid_Manager>();
    path = new List<Transform>();
	}
	
	// Update is called once per frame
	void Update () {
		if (Input.GetKeyDown(KeyCode.F)){
      path = gridManager.BreadthFirst(transform.position, transform.position + transform.forward * 8);
      for (int i = 0; i < path.Count - 1; i++){
        Debug.DrawRay(Vector3.up + path[i].position, path[i+1].position - path[i].position, Color.yellow, 3f);
      }
    }
	}
}
