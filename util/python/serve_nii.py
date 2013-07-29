def serve_files(filenames, tr):
    ic = InfoClient('localhost', 15000, 'localhost', 15001)
    for idx, filename in enumerate(filenames):
        img = nb.load(filename)
        if len(img.shape) == 4:
            for i in range(img.shape[3]):
                hdr_bytes, data_bytes = ic.ei.from_image(img, i,
                                                         nt=img.shape[3],
                                                         mosaic=True)
                ic.send(hdr_bytes)
                sleep(.05)
                ic.send(data_bytes)
                sleep(tr)
        else:
            hdr_bytes, data_bytes = ic.ei.from_image(img, idx,
                                                     nt=len(filename),
                                                     mosaic=True)
            ic.send(hdr_bytes)
            sleep(.05)
            ic.send(data_bytes)
            sleep(tr)
    ic.send('<rt:done>')

def main(argv):
    pass

if __name__ == "__main__":
    sys.exit(main(sys.argv))
