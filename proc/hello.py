# coding:utf-8 
import requests

import bs4


def catalog():

    url = 'https://www.xxbiqudu.com/5_5710'
    # url = 'https://www.biqugeg.net/0_509/'
    r = requests.get(url).text
    # print(r)
    soup = bs4.BeautifulSoup(r,'lxml')
    wrapper = soup.findAll(id='wrapper')
    box_con = wrapper[0].find_all(class_='box_con')
    catalog_list = box_con[1].find_all(id='list')
    dd_list = catalog_list[0].find_all('dd')
    del dd_list[0:10]
    catalog_navel = []
    for i in dd_list:
        a = i.find_all('a')
        catalog_navel.append(a[0].get("href"))
    return catalog_navel





if __name__ == '__main__':
    # print(catalog())
    url = catalog()
    r = requests.get(url[1]).text  #这里可以修改为任意章节的url，第x章就是url[x]
    new_r = r.replace('<br/>','\n \n')
    soup = bs4.BeautifulSoup(new_r, 'lxml')
    wrapper = soup.findAll(id='wrapper')
    content_read = wrapper[0].find_all(class_='content_read')
    box_con = content_read[0].find_all(class_='box_con')
    content = box_con[0].find_all(id='content')
    print((box_con[0].find_all('h1'))[0].text)
    print(content[0].text)

