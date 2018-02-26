using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class D_Tile_Color : MonoBehaviour {
  public void SetColor(Color col){
    GetComponent<MeshRenderer>().material.color = col;
  }
}
