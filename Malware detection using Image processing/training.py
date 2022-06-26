import os
import numpy as np
from keras.models import Sequential
from keras.layers import Dense, Conv2D, Flatten, MaxPooling2D, BatchNormalization, Dropout
from sklearn.model_selection import train_test_split
from summary import summarize_diagnostics, plot_dataset, visualize_model
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'

#Choosing the size of input images.
h = 128
w = 128
#Choosing the proportion of dataset split.
test_size = 0.15
validation_size = 0.25
train_size = 0.6


#loading prepared samples.
train_img_list = np.load("img_list.npy")
labels = np.load("labels.npy")
#spliiting the dataset for training and testing.
train_x, test_x, train_y, test_y = train_test_split(np.asarray(train_img_list), np.asarray(labels), test_size=test_size, random_state=25)



print(f"No. of training samples: {train_x.shape[0]}")
print(f"No. of testing samples: {test_x.shape[0]}")
print(f"No. of training samples: {train_y.shape[0]}")
print(f"No. of testing samples: {test_y.shape[0]}")


#CNN model configuration.
input_shape = (h, w, 1)
model = Sequential()
#Conv2D Layers
model.add(Conv2D(32, (16, 16), padding='same',input_shape=input_shape, activation = 'relu'))
model.add(Conv2D(32, (16, 16), activation = 'relu'))
#Max Pooling Layer
model.add(MaxPooling2D(pool_size=(2, 2)))
#Normalization Layer
model.add(BatchNormalization())
#Conv2D Layers
model.add(Conv2D(32, (16, 16), padding='same', activation = 'relu'))
model.add(Conv2D(32, (16, 16), activation = 'relu'))
#Max Pooling
model.add(MaxPooling2D(pool_size=(2, 2)))
#Normalization Layer
model.add(BatchNormalization())
#Dropout Layer
model.add(Dropout(0.2))
#Flattening Layer
model.add(Flatten())
#Dense Layer
model.add(Dense(100, activation = 'relu'))
model.add(Dense(10, activation = 'relu'))
model.add(Dense(1, activation = 'sigmoid'))


#graphical illustration
visualize_model(model)


#compiling the model.
model.compile(optimizer='adam', 
              loss='binary_crossentropy', 
              metrics=['accuracy'])

model.summary()


#training the model.
epochs = 15
validation_split = validation_size / (train_size + validation_size)
history = model.fit(train_x, train_y,
              epochs = epochs,
              validation_split=validation_split,
              shuffle = True)


#testing the model with test dataset.
_, acc = model.evaluate(test_x, test_y)


#displaying summary
summarize_diagnostics(history)
plot_dataset([train_size, validation_size, test_size])
print(f"Testing accuracy is {acc * 100}%")


#saving the model.
model.save("trained_model.h5")
