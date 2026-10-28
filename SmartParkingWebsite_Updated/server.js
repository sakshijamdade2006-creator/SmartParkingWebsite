const http = require('http');
const fs = require('fs');
const path = require('path');
const { execFile } = require('child_process');
const ROOT = __dirname;
const PUBLIC = path.join(ROOT, 'public');
const BIN = path.join(ROOT, process.platform === 'win32' ? 'parking.exe' : 'parking');
const mime = {'.html':'text/html; charset=utf-8','.css':'text/css; charset=utf-8','.js':'text/javascript; charset=utf-8','.ico':'image/x-icon'};
function runC(args) { return new Promise((resolve,reject)=>execFile(BIN,args,{cwd:ROOT,windowsHide:true},(err,stdout,stderr)=>{if(err)return reject(new Error(stderr||err.message));try{resolve(JSON.parse(stdout));}catch(e){reject(new Error('C backend returned invalid data: '+stdout));}})); }
const server = http.createServer(async (req,res)=>{
  const url = new URL(req.url,'http://localhost:3000');
  if(req.method==='GET' && url.pathname==='/api/state') { try { const data=await runC(['state']); res.writeHead(200,{'Content-Type':'application/json'}); return res.end(JSON.stringify(data)); } catch(e){res.writeHead(500,{'Content-Type':'application/json'});return res.end(JSON.stringify({error:e.message}));} }
  if(req.method==='POST' && (url.pathname==='/api/add'||url.pathname==='/api/exit')) { let body=''; req.on('data',c=>body+=c); req.on('end',async()=>{try{const d=JSON.parse(body||'{}'); const plate=String(d.plate||'').trim().replace(/\s+/g,'').toUpperCase(); if(!plate||plate.length>19||!/^[A-Z0-9-]+$/.test(plate)){res.writeHead(400,{'Content-Type':'application/json'});return res.end(JSON.stringify({ok:false,message:'Enter a valid registration number (letters/numbers only).'}));}const args=[url.pathname==='/api/add'?'add':'exit',plate]; if(url.pathname==='/api/add'){const slot=Number(d.slot||0);if(!Number.isInteger(slot)||slot<0||slot>12){res.writeHead(400,{'Content-Type':'application/json'});return res.end(JSON.stringify({ok:false,message:'Choose a valid slot from 1 to 12.'}));}args.push(String(slot));} const out=await runC(args);res.writeHead(200,{'Content-Type':'application/json'});res.end(JSON.stringify(out));}catch(e){res.writeHead(500,{'Content-Type':'application/json'});res.end(JSON.stringify({ok:false,message:e.message}));}});return; }
  let file = url.pathname==='/' ? 'index.html' : decodeURIComponent(url.pathname.slice(1));
  const full = path.resolve(PUBLIC,file); if(!full.startsWith(path.resolve(PUBLIC)+path.sep)){res.writeHead(403);return res.end('Forbidden');}
  fs.readFile(full,(err,data)=>{if(err){res.writeHead(404);return res.end('Not found');}res.writeHead(200,{'Content-Type':mime[path.extname(full)]||'application/octet-stream'});res.end(data);});
});
server.listen(3000,'127.0.0.1',()=>console.log('ParkSmart is running at http://localhost:3000'));
