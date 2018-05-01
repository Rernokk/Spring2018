using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum BladeState { IDLE, FIRED, RETURNING };

public class Blade_Controller : MonoBehaviour
{
  public static int step = 0;
  public BladeState currentState = BladeState.IDLE;
  public Rigidbody rgd;

  Blade_Controller ctrl;
  Coroutine returning;

  void Awake()
  {
    ctrl = GetComponent<Blade_Controller>();
    rgd = GetComponent<Rigidbody>();
  }
  // Update is called once per frame
  void Update()
  {

  }

  public void LaunchProjectile(Vector3 dir)
  {
    rgd.useGravity = true;
    transform.LookAt(transform.position + dir);
    rgd.AddForce(transform.forward * 30, ForceMode.Impulse);
  }
}
