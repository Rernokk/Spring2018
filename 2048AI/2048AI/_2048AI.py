import tensorFlow as tf
hello = tf.constant('Hllo TensorFlow!')
sess = tf.Session()
print(sess.run(hello))