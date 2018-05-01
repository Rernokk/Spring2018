using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player_Controller : MonoBehaviour
{
  [SerializeField]
  GameObject SpectralBladePrefab;

  [Header("Blade Positions")]
  [SerializeField]
  List<GameObject> OneBlade;
  [SerializeField]
  List<GameObject> TwoBlades;
  [SerializeField]
  List<GameObject> ThreeBlades;
  [SerializeField]
  List<GameObject> FourBlades;
  [SerializeField]
  List<GameObject> FiveBlades;

  Blade_Controller activeSpectralBlades;
  // Use this for initialization
  void Start()
  {
  }

  // Update is called once per frame
  void Update()
  {
    if (Input.GetKeyDown(KeyCode.Mouse1) && activeSpectralBlades == null)
    {
      activeSpectralBlades = Instantiate(SpectralBladePrefab, transform.position + Vector3.up * 1.5f + (transform.right * (Random.Range(-.1f, .1f))), transform.rotation).GetComponent<Blade_Controller>();
    }
    if (Input.GetKeyDown(KeyCode.Mouse0) && activeSpectralBlades != null)
    {
      activeSpectralBlades.LaunchProjectile(transform.forward);
      activeSpectralBlades = null;
    }

    if (Input.GetKey(KeyCode.A))
    {
      transform.Rotate(new Vector3(0, 30f * Time.deltaTime, 0));
    }
    if (Input.GetKey(KeyCode.D))
    {
      transform.Rotate(new Vector3(0, 30f * Time.deltaTime, 0));
    }
  }

  void LateUpdate()
  {
    Blade_Controller.step++;
  }
}
