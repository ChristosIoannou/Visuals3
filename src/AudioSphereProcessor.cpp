//#include "AudioSphereProcessor.h"
//
//void AudioSphereProcessor::declareSettings()
//{
//    params.setName(name);
//    params.add(posDecayRate.set("Decay Rate", 0.995, 0.99, 0.9999));
//}
//
//void AudioSphereProcessor::setup()
//{
//    //generate the mesh points
//    buildSphereMesh();
//    cout << "nverts: " << sphereMesh.getNumVertices() << endl;
//    cout << "arb: " << ofGetUsingArbTex() << ", norm: " << ofGetUsingNormalizedTexCoords() << endl;
//
//    shader.load("tex330.vert", "tex330.frag");
//    posShader.load("position330.vert", "position330.frag");
//
//    fboResolution = sphereResolution;      //for the sphere we set this to the resolution which = #of verts along each axis
//
//    //init the fbo's with blank data
//    vector<ofVec3f> fbo_init_data;
//    fbo_init_data.assign(fboResolution * fboResolution, ofVec3f(0.0, 0.0, 0.0));
//
//    posBuffer.allocate(fboResolution, fboResolution, GL_RGB32F);
//    posBuffer.src->getTextureReference().loadData((float*)&fbo_init_data[0], fboResolution, fboResolution, GL_RGB);
//    posBuffer.dst->getTextureReference().loadData((float*)&fbo_init_data[0], fboResolution, fboResolution, GL_RGB);
//
//    //reuse fbo_init_data for no real reason, it just needs to be blank
//    eqTexture.allocate(fboResolution, 1, GL_RGB32F_ARB);
//    eqTexture.loadData((float*)&fbo_init_data[0], fboResolution, 1, GL_RGB);
//
//    axisLocation = fboResolution;
//    angleIncrement = 180.0 / (float)fboResolution;
//
//    memset(bins, 0x00, sizeof(bins));
//}
//
//void AudioSphereProcessor::update(ContextPtr& context) 
//{
//    auto barkSpectrum = context->getResult<std::vector<float>>("barkSpectrum");
//
//    for (int i = 0; i < barkSpectrum.size(); ++i)
//    {
//        bins[i] = barkSpectrum[i];
//    }
//
//    /* put the eq vals into a path to turn them into a curve */
//    int line_len = fboResolution;
//    float ppseg = line_len / (float)(BARK_MAX + 1);
//    eqPath.clear();
//    eqPath.curveTo(0, -bins[0]);
//    for (int i = 0; i < BARK_MAX; i++)
//    {
//        eqPath.curveTo(i * ppseg, -bins[i]);
//    }
//
//    //smooth this out a little at the end so the eq texture wraps, 25 = BARK_MAX
//    //eqPath.curveTo(25 * ppseg, -(bins[0] + bins[BARK_MAX] + bins[BARK_MAX - 1] + bins[BARK_MAX - 2]) / 4.0f);
//    //eqPath.curveTo(26 * ppseg, -bins[0]);
//    //eqPath.curveTo(26 * ppseg, -bins[0]);
//
//    ofMesh eq_m = eqPath.getTessellation();
//
//    eqTexture.loadData((float*)eq_m.getVerticesPointer(), fboResolution, 1, GL_RGB); //load up the eq curve into a texture
//    axisLocation--;     //update where on the axis we will apply the latest eq data
//
//    if (axisLocation < 0)
//        axisLocation = fboResolution;
//
//    //use fbo to work out displacement coeffcients
//    posBuffer.dst->begin();
//    ofClear(0);
//    posShader.begin();
//    posShader.setUniformTexture("u_prevDisp", posBuffer.src->getTextureReference(), 0);
//    posShader.setUniformTexture("u_newDisp", eqTexture, 1); //pass the new displacement data
//    posShader.setUniform1f("u_axisloc", axisLocation);
//    posShader.setUniform1f("u_decayRate", posDecayRate);
//
//    ofSetColor(255, 255, 255, 255);
//    glBegin(GL_QUADS);
//    glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
//    glTexCoord2f(fboResolution, 0); glVertex3f(fboResolution, 0, 0);
//    glTexCoord2f(fboResolution, fboResolution); glVertex3f(fboResolution, fboResolution, 0);
//    glTexCoord2f(0, fboResolution); glVertex3f(0, fboResolution, 0);
//    glEnd();
//
//    posShader.end();
//    posBuffer.dst->end();
//    posBuffer.swap();
//}
//
//void AudioSphereProcessor::draw() 
//{
//    ofEnableDepthTest();
//    ofPushMatrix();
//    ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
//
//    //if (autoRotate) {
//    //    ofRotateY(-30);
//    //    if (rotateSin)
//    //        rotationSpeed = 2 * sin(ofGetFrameNum() * 0.005);
//    //    ofRotateX(startOffsetAngle += angleIncrement * rotationSpeed);
//    //    ofRotateZ(ofGetFrameNum() * 0.005);
//    //}
//    //else {
//        ofRotateX(0.0);
//        ofRotateY(0.0);
//        ofRotateZ(0.0);
//    //}
//
//    glEnable(GL_POINT_SMOOTH);
//    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
//
//    shader.begin();
//    shader.setUniformTexture("u_OffMap", posBuffer.src->getTextureReference(), 0);
//    shader.setUniform1f("u_fboRes", (float)fboResolution);
//
//    sphereMesh.drawVertices();
//    shader.end();
//    glDisable(GL_POINT_SMOOTH);
//    ofPopMatrix();
//    ofDisableDepthTest();
//}
//
//void AudioSphereProcessor::buildSphereMesh() 
//{
//    int n = sphereResolution * 2;
//    float ndiv2 = (float)n / 2;
//
//    float theta2 = TWO_PI;
//    float phi1 = -HALF_PI;
//    float phi2 = HALF_PI;
//    float r = sphereRadius;
//    sphereMesh.clear();
//    int i, j;
//    float theta1 = 0.f;
//    float jdivn, j1divn, idivn, dosdivn, unodivn = 1 / (float)n, t1, t2, t3, cost1, cost2, cte1, cte3;
//    cte3 = (theta2 - theta1) / n;
//    cte1 = (phi2 - phi1) / ndiv2;
//    dosdivn = 2 * unodivn;
//    ofVec3f e, p, e2, p2;
//
//    if (n < 0) {
//        n = -n;
//        ndiv2 = -ndiv2;
//    }
//    if (n < 4) { n = 4; ndiv2 = (float)n / 2; }
//    if (r <= 0) r = -r;
//
//    t2 = phi1;
//    cost2 = cos(phi1);
//    j1divn = 0;
//
//    ofVec3f vert, normal;
//    ofVec2f tcoord;
//
//    for (j = 0; j < ndiv2; j++) {
//        t1 = t2;
//        t2 += cte1;
//        t3 = theta1 - cte3;
//        cost1 = cost2;
//        cost2 = cos(t2);
//        e.y = sin(t1);
//        e2.y = sin(t2);
//        p.y = r * e.y;
//        p2.y = r * e2.y;
//
//        idivn = 0;
//        jdivn = j1divn;
//        j1divn += dosdivn;
//        for (i = 0; i <= n; i++) {
//            t3 += cte3;
//            e.x = cost1 * cos(t3);
//            e.z = cost1 * sin(t3);
//            p.x = r * e.x;
//            p.z = r * e.z;
//
//            normal.set(e.x, e.y, e.z);
//            tcoord.set(idivn, jdivn);
//            vert.set(p.x, p.y, p.z);
//
//            sphereMesh.addNormal(normal);
//            sphereMesh.addTexCoord(tcoord);
//            sphereMesh.addVertex(vert);
//
//            e2.x = cost2 * cos(t3);
//            e2.z = cost2 * sin(t3);
//            p2.x = r * e2.x;
//            p2.z = r * e2.z;
//
//            normal.set(e2.x, e2.y, e2.z);
//            tcoord.set(idivn, j1divn);
//            vert.set(p2.x, p2.y, p2.z);
//
//            sphereMesh.addNormal(normal);
//            sphereMesh.addTexCoord(tcoord);
//            sphereMesh.addVertex(vert);
//
//            idivn += unodivn;
//
//        }
//    }
//}
