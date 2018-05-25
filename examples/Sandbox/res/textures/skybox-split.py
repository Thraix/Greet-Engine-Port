from PIL import Image



im = Image.open("skybox.png")
width, height = im.size

im.crop((width/4,0,width/4*2,height/3)).save("skybox-top.png");
im.crop((width/4,height/3*2,width/4*2,height)).save("skybox-bottom.png");


im.crop((0,height / 3,width/4,height/3*2)).save("skybox-right.png");
im.crop((width/4*2,height / 3, width/4*3,height/3*2)).save("skybox-left.png");
im.crop((width/4*1,height / 3,width/4*2,height/3*2)).save("skybox-back.png");
im.crop((width/4*3,height / 3, width,height/3*2)).save("skybox-front.png");

