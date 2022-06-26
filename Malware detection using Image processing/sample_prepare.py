import os
import numpy as np
from os import listdir
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'

#provide path for malware and benign file directories
ben_path = 'D:/Semester 6/Project LAB/ml-sample-pack-small/benign/arm'
mal_path = 'D:/Semester 6/Project LAB/ml-sample-pack-small/malware/arm'


#objective size of image
h = 128
w = 128


#receives an array, processes averaging to resize the array and appends it to the image list.
def averaging(arr, append_arr, h, w) :
    new_arr = [] 
    temp = len(arr) / (h * w)
    #'if' case for choosing the appropriate pool size.
    if(temp < 1) : #in case the array is smaller than the requested size.
        pool_size = 1
    else : 
        pool_size = round(temp)
    max_range = len(arr)
    sum = 0
    #main loop for averaging.
    for i in range(0, max_range, pool_size): 
        #'if' case for ensuring no out-of-bounds array access.
        if i + pool_size < max_range : 
            for j in range(i, i + pool_size): #average the values in a single pool.
                sum += arr[j]
            new_arr.append(np.uint8(sum / pool_size))
        else :
            for j in range(i, max_range):
                sum += arr[j]
            new_arr.append(np.uint8(sum / (max_range - i)))
        sum = 0
    #'if' case for filling up empty space or cutting the array to the correct size.
    if(len(new_arr) < h*w):
        zeros_length = h*w - len(new_arr)
        img_arr_stacked = np.hstack((new_arr, np.zeros(zeros_length, np.uint8)))
        append_arr.append(img_arr_stacked)
    else :
        append_arr.append(new_arr[0:(h*w)])


#receives the requested image size
#and the directory of the files, from which the samples are prepared.
def prepare_samples(path, h, w) :
    avgd_arr = []
    iteration = 0
    print("The samples are being prepared, please wait!")
    for files in listdir(path) : #reads each file.
        with open(os.path.join(path, files), 'rb') as open_file:
            data = open_file.read()
            data_arr = np.frombuffer(data, dtype=np.uint8) #reads the file bytes.
            averaging(data_arr, avgd_arr, h, w) #averages the array to the requested image size.
            iteration+=1
    #changing the dimensions appropriate for model input.
    samples_list = np.zeros(shape = (len(avgd_arr), h,w,1), dtype = np.uint8)
    for i in range(len(avgd_arr)):
        samples_list[i,:,:,0] = np.reshape(list(avgd_arr[i]), (h,w))   
    print(f"Finished preparing {iteration} samples")
    return samples_list #returns the image list.


#reads files and prepares samples.
ben_img_list = prepare_samples(ben_path, h, w)
mal_img_list = prepare_samples(mal_path, h, w)


#ensures the numbers of malware and non-malware images match. 
if len(ben_img_list) > len(mal_img_list) :
    ben_img_list = ben_img_list[0:len(mal_img_list)]
else :
    mal_img_list = mal_img_list[0:len(ben_img_list)]


#creates target values/data.
labels = [0 for _ in ben_img_list] + [1 for _ in mal_img_list]


#creates a final image list with both types of files.
train_img_list = np.vstack((ben_img_list, mal_img_list))
#further reshaping for model input.
train_img_list = train_img_list.astype('float32')
train_img_list /= 255


#saves the data in 
np.save('img_list.npy', train_img_list)
np.save('labels.npy', labels)
