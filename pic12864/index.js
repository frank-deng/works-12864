const Jimp=require('jimp'),
  RGBQuant = require('rgbquant'),
  SerialPort = require('serialport');

async function main(argv){
  let image=await Jimp.read(argv[2]);
  image.resize(128, 64).greyscale();
  let quant=new RGBQuant({
    colors:2,
    palette:[
      [0,0,0],
      [255,255,255]
    ],
    dithKern:'FloydSteinberg',
    dithSerp:true,
    useCache:false
  });
  quant.sample(image.bitmap.data,image.bitmap.width);
  let result=quant.reduce(image.bitmap.data, 1, 'FloydSteinberg', true);

  let commands=[[0x01]];
  for(let row=0; row<8; row++){
    for(let col=0; col<16; col++){
      let partCommand=[];
      partCommand.push((row<<4|col)|0x80);
      for(let x=0; x<8; x++){
        let data=0;
        for(let y=0; y<8; y++){
          let pixel=result[((y+row*8)*128+(x+col*8))*4];
          if(pixel>127){
            data|=(1<<y);
          }
        }
        partCommand.push(data);
      }
      commands.push(partCommand);
    }
  }
  
  /*
  let resultImg=await Jimp.read({
    data:result,
    width:128,
    height:64
  });
  resultImg.write('output.bmp');
  */
  //let baudRate=57600;
  let baudRate=115200;
  let serial = new SerialPort('\\\\.\\COM7',{baudRate});
  await new Promise(resolve=>{
    serial.on('open',resolve);
  });
  for(let item of commands){
    serial.write(Buffer.from(item));
    await new Promise(resolve=>{
      setTimeout(resolve,8);
    });
  }
  //serial.write(Buffer.from([0x81, 1,3,7,15,0x1f, 0x3f, 0x7f, 0xff]));
}

main(process.argv);

