'use strict';

const fs = require('fs');

console.log('starting');

const inFile = 'WiFiManager.template.html';
const outFile = 'template.h';

const defineRegEx = /<!-- ([A-Z_]+) -->/gm;
console.log('parsing', inFile);

fs.readFile(inFile, 'utf8', function (err,data) {
  if (err) {
    return console.log(err);
  }
  //console.log(data);

  let defines = data.match(defineRegEx);

  //console.log(defines);
  var stream = fs.createWriteStream(outFile);
  stream.once('open', function(fd) {
    for (const i in defines) {

      const start = defines[i];
      const end = start.replace('<!-- ', '<!-- /')
      defineRegEx.lastIndex = 0;
      const constantName = defineRegEx.exec(start)[1];

      console.log(constantName);
      var extractRE = new RegExp(start + '([\\s\\S]+)' + end, 'gm');
      let extractArray = extractRE.exec(data);
      if(extractArray.length > 1) {
        let def = extractArray[1];
        //console.log(def);
        //minimise a bit
        def = def.replace(/\s+/g, ' ');
        def = def.replace(/>\s+</g, '><');
        def = def.trim();
        //more extraneous spaces - possible bad results, needs to be checked
        //def = def.replace(/(\w)\s(\W)|(\W)\s(\w)|(\W)\s(\W)/g, '$1$2$3$4$5$6');
        def = def.replace(/(\w)\s(\W)|(\W)\s(\w)/g, '$1$2$3$4');
        //escape double quotes
        def = def.replace(/\\([\s\S])|(")/g, "\\$1$2");


        console.log(def);
        //const char HTTP_HEAD[] PROGMEM            =
        let string = 'const char ' + constantName + '[] PROGMEM';
        for (let i = string.length; i < 42; i++) {
          string += ' ';
        }
        string += '= "' + def + '";\n';
        stream.write(string);
      }
    }
    stream.end();
  });
});
