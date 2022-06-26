import tensorflow as tf
import os
tf.compat.v1.disable_eager_execution()
import numpy as np
from art.attacks.evasion import FastGradientMethod
from art.estimators.classification import KerasClassifier
from sklearn.model_selection import train_test_split
from keras.models import load_model
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'

# Step 1: Load the dataset
train_img_list = np.load("img_list.npy")
labels = np.load("labels.npy")
x_train, x_test, y_train, y_test = train_test_split(np.asarray(train_img_list), np.asarray(labels), test_size=0.3, random_state=25)


# Step 2: Create the ART classifier
model = load_model("trained_model.h5")
classifier = KerasClassifier(model=model,clip_values=(0, 1), use_logits=False)


# Step 3: Train the ART classifier
classifier.fit(x_train, y_train, batch_size=64, nb_epochs=3)


# Step 4: Evaluate the ART classifier on benign test examples
predictions = classifier.predict(x_test)
print(f"{len(predictions)} predictions and {len(y_test)} results")
predictions = np.around(predictions.flatten())
accuracy = (np.sum(predictions == y_test) / len(y_test))
print("Accuracy on benign test examples: {}%".format(accuracy * 100))


# Step 5: Generate adversarial test examples
attack = FastGradientMethod(estimator=classifier, eps=0.2)
x_train_adv = attack.generate(x=x_train)
x_test_adv = attack.generate(x=x_test)


# Step 6: Evaluate the ART classifier on adversarial examples
predictions = classifier.predict(x_test_dev)
print(f"{len(predictions)} predictions and {len(y_test)} results")
predictions = np.around(predictions.flatten())
accuracy = (np.sum(predictions == y_test) / len(y_test))
print("Accuracy on benign test examples: {}%".format(accuracy * 100))


# Step 7: Re-train on dataset with adversial examples
x_train_added = np.concatenate((x_train, x_train_adv), axis=0)
y_train_added = np.concatenate((y_train, y_train), axis = 0)
classifier.fit(x_train_added, y_train_added, batch_size=64, nb_epochs=3)


# Step 8: Re-Evaluate the ART classifier on adversial test examples
predictions = classifier.predict(x_test_adv)
predictions = np.around(predictions.flatten())
accuracy = (np.sum(predictions == y_test) / len(y_test))
print("Accuracy on adversarial test examples: {}%".format(accuracy * 100))
