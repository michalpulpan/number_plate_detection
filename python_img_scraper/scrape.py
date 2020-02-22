from selenium import webdriver
import wget


driver = webdriver.Chrome(executable_path='/Users/pulpan/Documents/C++/number_plate_recognition_SVM/python_img_scraper/chromedriver')


driver.get('https://www.miloscermak.com/spz/')

sez=[]

images = driver.find_elements_by_tag_name('img')
for image in images:
    str = image.get_attribute('data-image')
    indexofslash = str.rfind('/')
    indexofdot = str.rfind('.')
    strName = str[indexofslash+1:indexofdot:]
    if(strName != 'image-asset' and not(strName in sez)):
        sez.append(strName)
        print(strName)
        wget.download(str)
    elif(strName == 'mage-asset'):
        wget.download(str)
 

driver.close()